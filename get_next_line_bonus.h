/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpontici <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 21:01:23 by rpontici          #+#    #+#             */
/*   Updated: 2025/01/08 21:41:23 by rpontici         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_chunk
{
	char			*data;
	size_t			size;
	struct s_chunk	*next;
}	t_chunk;

typedef struct s_fdata
{
	char	buffer[BUFFER_SIZE];
	size_t	start;
	size_t	end;
	t_chunk	*head;
	t_chunk	*tail;
}	t_fdata;

char	*get_next_line(int fd);
t_fdata	*ft_newindex(void);
t_fdata	*ft_updateindex(t_fdata *index, int fd);
int		ft_read_and_collect(t_fdata *index, int *found_newline);
char	*ft_join_chunks(t_fdata *index);
void	ft_free_chunks(t_fdata *index);
void	ft_add_chunk(t_fdata *index, const char *src, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);

#endif
