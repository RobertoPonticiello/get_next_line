/* ************************************************************************** */
/*                                                                            */
/*                            get_next_line_utils.c                           */
/* ************************************************************************** */

#include "get_next_line.h"

/* 
** 1) ft_updateindex(): legge dal file se il buffer è finito
*/
t_fdata *ft_updateindex(t_fdata *index, int fd)
{
    ssize_t bytes_read;

    if (index->start >= index->end)
    {
        bytes_read = read(fd, index->buffer, BUFFER_SIZE);
        if (bytes_read < 0)
        {
            // Errore di lettura
            free(index);
            return NULL;
        }
        if (bytes_read == 0)
        {
            // EOF
            index->start = 0;
            index->end = 0;
        }
        else
        {
            index->start = 0;
            // Converte ssize_t in size_t
            index->end = (size_t)bytes_read;
        }
    }
    return (index);
}

/*
** 2) ft_add_chunk(): aggiunge un pezzo (chunk) in coda alla lista
*/
void ft_add_chunk(t_fdata *index, const char *src, size_t len)
{
    t_chunk *new_chunk;

    new_chunk = (t_chunk *)malloc(sizeof(t_chunk));
    if (!new_chunk)
        return;
    new_chunk->data = (char *)malloc(sizeof(char) * len);
    if (!new_chunk->data)
    {
        free(new_chunk);
        return;
    }
    ft_memcpy(new_chunk->data, src, len);
    new_chunk->size = len;
    new_chunk->next = NULL;

    if (!index->head)  // Lista vuota
        index->head = new_chunk;
    else
        index->tail->next = new_chunk;

    index->tail = new_chunk;
}

/*
** 3) ft_read_and_collect(): cerca '\n' e colleziona i byte nei chunk
**    Se trova '\n', imposta found_newline=1 e si ferma.
**    Altrimenti aggiunge tutto e "consuma" il buffer.
*/
int ft_read_and_collect(t_fdata *index, int *found_newline)
{
    size_t i = 0;

    *found_newline = 0;

    // Se end == 0 => niente da leggere => EOF
    if (index->end == 0)
        return 0;

    // Scansiona il buffer
    while ((index->start + i) < index->end)
    {
        if (index->buffer[index->start + i] == '\n')
        {
            // Aggiungiamo al chunk i caratteri [start..start+i], compreso '\n'
            ft_add_chunk(index, &index->buffer[index->start], i + 1);
            *found_newline = 1;

            // Avanziamo lo start
            index->start += (i + 1);
            return 1;
        }
        i++;
    }
    // Se NON abbiamo trovato '\n', aggiungiamo tutto e consumiamo il buffer
    ft_add_chunk(index, &index->buffer[index->start], i);
    index->start += i;
    return 1;
}

/*
** 4) ft_join_chunks(): unisce tutti i chunk in un'unica stringa
*/
char *ft_join_chunks(t_fdata *index)
{
    t_chunk *tmp;
    size_t total_size = 0;
    size_t offset = 0;
    char *result;

    // 1) Calcola la dimensione totale
    tmp = index->head;
    while (tmp)
    {
        total_size += tmp->size;
        tmp = tmp->next;
    }

    // 2) Alloca la stringa (total_size + 1 per il '\0')
    result = (char *)malloc(sizeof(char) * (total_size + 1));
    if (!result)
        return NULL;

    // 3) Copia i chunk
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

/*
** 5) ft_free_chunks(): libera tutti i nodi chunk
*/
void ft_free_chunks(t_fdata *index)
{
    t_chunk *cur = index->head;
    t_chunk *next;

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

/*
** 6) ft_strjoin(): se la usi da qualche parte (non obbligatoria nel chunk approach)
*/
char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!result)
		return (NULL);
	while (i < ft_strlen(s1))
	{
		result[i] = s1[i];
		i++;
	}
	while (j < ft_strlen(s2))
	{
		result[i + j] = s2[j];
		j++;
	}
	result[i + j] = '\0';
	if (s1)
		free(s1);
	return (result);
}

/* 7) Se non ti serve davvero, rimuovi pure set_i o ft_readline() 
**    Qui li lasciamo commentati in caso volessi un approccio "classico".
*/
// size_t set_i(t_fdata *index) {...}
// char *ft_readline(...) {...}

/*
** 8) ft_clearindex(): libera la struttura index se serve (qui non sempre usata)
*/
void	ft_clearindex(t_fdata *index)
{
	if (index)
		free(index);
}

/*
** 9) ft_newindex(): alloca e inizializza un t_fdata
*/
t_fdata *ft_newindex(void)
{
    t_fdata *result;

    result = (t_fdata *)malloc(sizeof(t_fdata));
    if (!result)
        return NULL;
    result->start = 0;
    result->end = 0;
    result->head = NULL;
    result->tail = NULL;
    return (result);
}

/*
** 10) ft_strlen() e ft_memcpy()
*/
size_t ft_strlen(const char *s)
{
    size_t i = 0;
    while (s && s[i])
        i++;
    return i;
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
    size_t i;
    char *d = (char *)dst;
    const char *s = (const char *)src;

    if (!dst && !src)
        return NULL;
    i = 0;
    while (i < n)
    {
        d[i] = s[i];
        i++;
    }
    return dst;
}
