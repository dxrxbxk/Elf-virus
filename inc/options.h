#ifndef OPTIONS_H
# define OPTIONS_H

# include <unistd.h>
# include <string.h>

#define OPTIONS "a:p:o:v"

typedef struct s_options {
	int	algo;
	char	*binary;
	char	*payload;
} t_options;

t_options *options_handler(int argc, char **argv);

#endif
