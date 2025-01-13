/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 19:45:10 by rpontici          #+#    #+#             */
/*   Updated: 2025/01/13 20:18:07 by rpontici         ###   ########.fr       */
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
	int		found_newline;
	int		read_ret;
	char	*line;

	found_newline = 0;
	line = NULL;
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
	line = ft_join_chunks(info);
	ft_free_chunks(info);
	return (line);
}

char	*get_next_line(int fd)
{
	char	*line;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!g_array[fd])
	{
		g_array[fd] = ft_newindex();
		if (!g_array[fd])
			return (NULL);
	}
	line = gnl_loop(g_array[fd], fd);
	if (!line)
	{
		ft_free_chunks(g_array[fd]);
		free(g_array[fd]);
		g_array[fd] = NULL;
	}
	return (line);
}

__attribute__((destructor))

static void	gnl_cleanup(void)
{
	int	i;

	i = 0;
	while (i < 1024)
	{
		if (g_array[i])
		{
			ft_free_chunks(g_array[i]);
			free(g_array[i]);
			g_array[i] = NULL;
		}
		i++;
	}
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

	while (count < 5)
	{
		line = get_next_line(fd);
		printf("%d) %s", count, line);
		free(line);
		count++;
	}
	return (0);
}*/
