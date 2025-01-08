#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>   // per open(), se serve
# include <stdio.h>   // per test

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

// Struttura per i "pezzi" (chunk) letti
typedef struct s_chunk
{
    char            *data;       // Blocco di caratteri
    size_t          size;        // Quantità di byte in "data"
    struct s_chunk  *next;       // Puntatore al chunk successivo
}   t_chunk;

// Struttura per tracciare stato del file descriptor
typedef struct s_fdata
{
    char    buffer[BUFFER_SIZE]; // Buffer di lettura
    size_t start;               // Indice di inizio dati validi
    size_t end;                 // Indice di fine dati validi
    t_chunk *head;               // Testa della lista di chunk
    t_chunk *tail;               // Coda della lista di chunk
}   t_fdata;

// Funzioni principali
char    *get_next_line(int fd);

// Funzioni "util"
t_fdata *ft_newindex(void);
t_fdata *ft_updateindex(t_fdata *index, int fd);
int ft_read_and_collect(t_fdata *index, int *found_newline);
char    *ft_join_chunks(t_fdata *index);
void    ft_free_chunks(t_fdata *index);
void    ft_add_chunk(t_fdata *index, const char *src, size_t len);

// Mini-funzioni di supporto (se non hai una tua libft)
size_t  ft_strlen(const char *s);
void    *ft_memcpy(void *dst, const void *src, size_t n);

#endif
