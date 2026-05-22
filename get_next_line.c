/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakamot <dnakamot@student.42.tokyo.jp>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 01:44:31 by dnakamot          #+#    #+#             */
/*   Updated: 2026/05/22 02:19:45 by dnakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*append_buf(char **line, char *buf, int n)
{
	size_t	old_len;
	char	*new_line;

	old_len = ft_strlen(*line);
	new_line = ft_realloc(*line, old_len + 1, old_len + n + 1);
	if (!new_line)
	{
		free(*line);
		*line = NULL;
		return (NULL);
	}
	*line = new_line;
	ft_memcpy(*line + old_len, buf, n + 1);
	return (*line);
}

static int	fill_line(int fd, char **line)
{
	char	*buf;
	int		n;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (-1);
	n = 0;
	while (!ft_strchr(*line, '\n'))
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n <= 0)
			break ;
		buf[n] = '\0';
		if (!append_buf(line, buf, n))
		{
			free(buf);
			return (-1);
		}
	}
	free(buf);
	return (n);
}

static char	*ret_line(t_gnl *g)
{
	size_t	len;

	if (g->nl)
		len = (size_t)(g->nl - g->line + 1);
	else
		len = ft_strlen(g->line);
	g->ret = malloc(len + 1);
	if (!g->ret)
		return (NULL);
	ft_memcpy(g->ret, g->line, len);
	g->ret[len] = '\0';
	return (g->ret);
}

static void	save_nextline(t_gnl *g)
{
	char	*remainder;

	if (g->nl && *(g->nl + 1) != '\0')
	{
		remainder = ft_strdup(g->nl + 1);
		free(g->line);
		g->line = remainder;
	}
	else
	{
		free(g->line);
		g->line = NULL;
	}
}

char	*get_next_line(int fd)
{
	static t_gnl	g;
	int				n;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	n = fill_line(fd, &g.line);
	if (n < 0 || !g.line || !g.line[0])
	{
		free(g.line);
		g.line = NULL;
		return (NULL);
	}
	g.nl = ft_strchr(g.line, '\n');
	if (!ret_line(&g) && g.line)
	{
		free(g.line);
		g.line = NULL;
		return (NULL);
	}
	save_nextline(&g);
	return (g.ret);
}

// int	main(void)
// {
// 	int fd;
// 	char *bufp;

// printf("--- 標準入力テスト (何か文字を入力してEnterを押してください) ---\n");

// bufp = get_next_line(0);
// printf("GNLが返した行: %s", bufp);
// free(bufp);

// printf("\n");

// 	printf("--- ファイル読み込みテスト ---\n");
// 	fd = open("text.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		printf("エラー: text.txt が開けませんでした。\n");
// 		return (1);
// 	}
// 	while ((bufp = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", bufp);
// 		free(bufp);
// 	}
// 	close(fd);
// 	return (0);
// }