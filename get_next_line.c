/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakamot <dnakamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:30:13 by dnakamot          #+#    #+#             */
/*   Updated: 2026/05/19 11:09:22 by dnakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_getchar(int fd)
{
	char	c;
	int		ret;

	ret = read(fd, &c, 1);
	if (ret <= 0)
		return (EOF);
	return ((unsigned char)c);
}

char	*get_next_line(int fd)
{
	char	*line;
	char	*new_line;
	int		c;
	int		len;
	int		i;

	line = malloc(1);
	if (!line)
		return (NULL);
	line[0] = '\0';
	len = 0;
	while ((c = ft_getchar(fd)) != EOF)
	{
		printf("line=%s\n", line);
		new_line = malloc(len + 2);
		if (!new_line)
			return (NULL);
		i = 0;
		while (i < len)
		{
			printf("ループ(i=%d:len=%d)\n", i, len);
			new_line[i] = line[i];
			i++;
		}
		new_line[len] = (char)c;
		new_line[len + 1] = '\0';
		printf("new_line%s\n", new_line);
		free(line);
		line = new_line;
		len++;
		if (c == '\n')
			break ;
	}
	printf("ループから出た");
	if (len == 0 && c == EOF)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

int	main(void)
{
	int fd;
	char *bufp;
	fd = open("text.txt", O_RDONLY);
	bufp = get_next_line(fd);
	printf("%s", bufp);
	bufp = get_next_line(fd);
	printf("%s", bufp);
	close(fd);
	return (0);
}