#ifndef INJECT_H
# define INJECT_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>

enum {
	SILVIO,
	ALGO_COUNT
} e_algo;

typedef struct s_options {
	int	algo;
	char	*fd_name;
} t_options;

#endif
