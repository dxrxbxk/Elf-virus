#ifndef MAP_H
# define MAP_H

# include <sys/mman.h>
# include <fcntl.h>
# include <stdint.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

#include "data.h"

uint8_t	*map_file(const char *filename, size_t *size);
uint8_t	*expand_file(uint8_t *file, size_t size, size_t new_size, t_data *data);

#endif
