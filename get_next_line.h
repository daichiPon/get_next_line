/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dnakamot <dnakamot@student.42.tokyo.jp>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 01:44:16 by dnakamot          #+#    #+#             */
/*   Updated: 2026/05/22 02:01:45 by dnakamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_gnl
{
	char	*line;
	char	*nl;
	char	*ret;
}			t_gnl;

size_t		ft_strlen(const char *s);
char		*ft_strchr(const char *s, int c);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char		*ft_strdup(const char *s);
char		*get_next_line(int fd);

#endif /* GET_NEXT_LINE_H */