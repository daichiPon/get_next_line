* This project has been created as part
of the 42 curriculum by dnakamot.*

# get_next_line

## 説明

`get_next_line` は、ファイルディスクリプタから1行ずつ読み込むC言語の関数です。呼び出すたびに次の1行（改行文字 `\n` を含む）を返し、読み込むものがなくなるかエラーが発生した場合は `NULL` を返します。

このプロジェクトの目的は、static変数・動的メモリ確保・複数回の関数呼び出しをまたぐバッファ管理という、システムプログラミングの核心となる概念を理解することです。

---

## 手順

### コンパイル

```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl
```

`BUFFER_SIZE` は任意の正の整数に変更できます。

```bash
gcc -D BUFFER_SIZE=1 ...      # 1バイトずつ読む
gcc -D BUFFER_SIZE=9999 ...
```

### 使用例

```c
#include "get_next_line.h"

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### ファイル構成

| ファイル | 内容 |
|---|---|
| `get_next_line.h` | ヘッダー — 構造体定義とプロトタイプ |
| `get_next_line.c` | メイン関数とヘルパー関数 |
| `get_next_line_utils.c` | ユーティリティ関数（strlen・strchrなど） |

---

## アルゴリズムの説明と選択理由

### 概要

`get_next_line` は複数回呼ばれるたびに、前回の呼び出しで読み込んだが返していない残りのデータを覚えておく必要があります。static変数は関数を抜けてもメモリが消えないため、この残りバッファの保持に最適です。

### データ構造

状態の管理に `t_gnl` 構造体を使用しています。

```c
typedef struct s_gnl
{
    char    *line;   // fdから読み込んだデータを蓄積するバッファ
    char    *nl;     // line内の'\n'の位置を指すポインタ（別途mallocしない）
    char    *ret;    // 呼び出し元に返す1行分の文字列
}   t_gnl;
```

3つの関連するポインタをひとまとめにすることで、ヘルパー関数が `t_gnl *` 1つを受け取るだけで全状態を操作できます。引数の数が減り、各関数を25行以内に収めることができます。

### 処理の流れ

```
1. fill_line      — '\n'が見つかるかEOFになるまでread()を繰り返し、
                    読んだデータをreallocでg.lineに追記する。
                    bufはヒープに確保（スタックオーバーフロー対策）。

2. extract_line   — g.lineから1行分だけを新しくmallocして切り出す。
                    '\n'があればそこまで、なければg.line全体を返す。

3. save_remainder — '\n'より後ろをft_strdupで新しくコピーし、
                    古いg.lineをfreeしてからg.lineに保存する。

4. g.retを返す   — 呼び出し元がfree()する責任を持つ。
```

### bufをmallocにした理由

`char buf[BUFFER_SIZE + 1]` はスタックに確保されます。スタックは通常8MBしかないため、`BUFFER_SIZE` が大きい場合（例: 10,000,000）は宣言だけでスタックオーバーフローします。`malloc` でヒープに確保することで、利用可能なメモリの限り対応できます。

### save_remainderでft_strdupを使う理由

`g->nl + 1` は `g->line` の内部を指すポインタです。先に `g->line` を `free` すると `g->nl + 1` がダングリングポインタになりデータが消えます。`ft_strdup` で先にコピーしてから `free` することで安全に残りデータを保存しています。

---

## 参考資料

- 42 Subject
- [man 2 read](https://man7.org/linux/man-pages/man2/read.2.html) — read() システムコール
- [ralloc](https://daeudaeu.com/c-realloc/) — 動的メモリ確保
- [Static 変数 in C言語](https://www.sejuku.net/blog/24205)
- [ファイルディスクリプタの理解](https://wa3.i-3-i.info/word14383.html)

### AIの使用について

このプロジェクトでは Claude（Anthropic）を以下の用途で使用しました。

- **デバッグ** — 複数行ファイルを読み込んだ際に1行目しか返されない原因の特定（行の切り出し処理と残りデータの保存方法のバグ修正）
- **コードレビュー** — 大きな `BUFFER_SIZE` でのスタックオーバーフローリスクの確認、normの25行制限への対応
- **概念の理解** — ポインタ演算（`g->nl - g->line`）、スタックとヒープの違い、関数・変数それぞれにおける `static` の役割の説明
- **README作成** — このドキュメントの構成と文章の作成

最終的な実装の判断とコードの確認はすべて本人が行いました。