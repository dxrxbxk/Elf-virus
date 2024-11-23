#ifndef INJECT_H
# define INJECT_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>

enum {
	SILVIO = 0,
	TEXT,
	ALGO_SIZE
} e_algo;

typedef struct s_options {
	int	algo;
	char	*binary;
	char	*payload;
} t_options;

#endif
