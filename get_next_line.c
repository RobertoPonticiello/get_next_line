/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:45:13 by rpontici          #+#    #+#             */
/*   Updated: 2025/01/08 20:51:37 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_fdata	*ft_newindex(void)
{
	t_fdata	*result;

	result = (t_fdata *)malloc(sizeof(t_fdata));
	if (!result)
		return (NULL);
	result->start = 0;
	result->end = 0;
	result->head = NULL;
	result->tail = NULL;
	return (result);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*d;
	const char	*s;

	d = (char *)dst;
	s = (const char *)src;
	if (!dst && !src)
		return (NULL);
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

static char	*gnl_loop(t_fdata *info, int fd)
{
	int	found_newline;
	int	read_ret;

	found_newline = 0;
	while (!found_newline)
	{
		info = ft_updateindex(info, fd);
		if (!info)
			return (NULL);
		if (info->end == 0)
			break ;
		read_ret = ft_read_and_collect(info, &found_newline);
		if (!read_ret)
			break ;
	}
	if (!info->head)
		return (NULL);
	return (ft_join_chunks(info));
}

char	*get_next_line(int fd)
{
	static t_fdata	*fd_array[1024];
	char			*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!fd_array[fd])
	{
		fd_array[fd] = ft_newindex();
		if (!fd_array[fd])
			return (NULL);
	}
	ft_free_chunks(fd_array[fd]);
	line = gnl_loop(fd_array[fd], fd);
	return (line);
}
/*
int	main(void)
{
	int fd;
	char *line;
	int count = 1;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("Errore apertura file");
		return (1);
	}

	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%d) %s", count, line);
		free(line);
		count++;
	}
	close(fd);
	return (0);
}*/