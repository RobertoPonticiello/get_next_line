/* ************************************************************************** */
/*                           get_next_line.c                                  */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_fdata	*fd_array[1024];
	t_fdata         *info;
	char            *line;
	int             found_newline;
	int             read_ret;

	// 1) Controlli di base
	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);

	// 2) Se è la prima volta su questo fd, creiamo la struttura
	if (!fd_array[fd])
	{
		fd_array[fd] = ft_newindex();
		if (!fd_array[fd])
			return (NULL);
	}
	info = fd_array[fd];

	// 3) Resettiamo i chunk prima di leggere una nuova riga
	ft_free_chunks(info);

	line = NULL;
	found_newline = 0;

	// 4) Loop finché non troviamo '\n' o EOF
	while (!found_newline)
	{
		// a) Leggiamo dal file se il buffer è vuoto
		info = ft_updateindex(info, fd);
		if (!info)    // Errore di lettura (read < 0)
			return (NULL);

		// b) Se end == 0 => siamo a EOF => stop
		if (info->end == 0)
			break;

		// c) Colleziona i byte nel buffer in un chunk 
		//    e controlla se c’è un '\n'
		read_ret = ft_read_and_collect(info, &found_newline);
		if (!read_ret)  
			break; 
	}

	// 5) Se la lista dei chunk è vuota => nessuna riga => NULL
	if (!info->head)
		return (NULL);

	// 6) Unisci i chunk in un'unica stringa
	line = ft_join_chunks(info);
	if (!line)
		return (NULL);

	// 7) Ritorna la linea accumulata
	return (line);
}
/*
int	main(void)
{
	int   fd;
	char *line;
	int   count = 1;

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