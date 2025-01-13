/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 20:20:28 by rpontici          #+#    #+#             */
/*   Updated: 2025/01/13 20:17:42 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_fdata	*ft_updateindex(t_fdata *index, int fd)
{
	ssize_t	bytes_read;

	if (index->start >= index->end)
	{
		bytes_read = read(fd, index->buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			return (NULL);
		}
		if (bytes_read == 0)
		{
			index->start = 0;
			index->end = 0;
		}
		else
		{
			index->start = 0;
			index->end = (size_t)bytes_read;
		}
	}
	return (index);
}

void	ft_add_chunk(t_fdata *index, const char *src, size_t len)
{
	t_chunk	*new_chunk;

	new_chunk = (t_chunk *)malloc(sizeof(t_chunk));
	if (!new_chunk)
		return ;
	new_chunk->data = (char *)malloc(sizeof(char) * len);
	if (!new_chunk->data)
	{
		free(new_chunk);
		return ;
	}
	ft_memcpy(new_chunk->data, src, len);
	new_chunk->size = len;
	new_chunk->next = NULL;
	if (!index->head)
		index->head = new_chunk;
	else
		index->tail->next = new_chunk;
	index->tail = new_chunk;
}

int	ft_read_and_collect(t_fdata *index, int *found_newline)
{
	size_t	i;

	i = 0;
	*found_newline = 0;
	if (index->end == 0)
		return (0);
	while ((index->start + i) < index->end)
	{
		if (index->buffer[index->start + i] == '\n')
		{
			ft_add_chunk(index, &index->buffer[index->start], i + 1);
			*found_newline = 1;
			index->start += (i + 1);
			return (1);
		}
		i++;
	}
	ft_add_chunk(index, &index->buffer[index->start], i);
	index->start += i;
	return (1);
}

char	*ft_join_chunks(t_fdata *index)
{
	t_chunk	*tmp;
	size_t	total_size;
	size_t	offset;
	char	*result;

	total_size = 0;
	offset = 0;
	tmp = index->head;
	while (tmp)
	{
		total_size += tmp->size;
		tmp = tmp->next;
	}
	result = (char *)malloc(sizeof(char) * (total_size + 1));
	if (!result)
		return (NULL);
	tmp = index->head;
	while (tmp)
	{
		ft_memcpy(result + offset, tmp->data, tmp->size);
		offset += tmp->size;
		tmp = tmp->next;
	}
	result[offset] = '\0';
	return (result);
}

void	ft_free_chunks(t_fdata *index)
{
	t_chunk	*cur;
	t_chunk	*next;

	cur = index->head;
	while (cur)
	{
		next = cur->next;
		free(cur->data);
		free(cur);
		cur = next;
	}
	index->head = NULL;
	index->tail = NULL;
}
