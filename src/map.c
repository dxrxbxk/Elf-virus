#include "map.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

uint8_t	*map_file(const char *filename, size_t *size) {
	int		fd;
	uint8_t	*file;
	ssize_t	ret;
	struct stat st;

	if ((fd = open(filename, O_RDONLY)) == -1) {
		perror("open");
		return NULL;
	}

	if (fstat(fd, &st) == -1) {
		close(fd);
		perror("fstat");
		return NULL;
	}

	file = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (file == MAP_FAILED) {
		close(fd);
		perror("mmap");
		return NULL;
	}

	close(fd);

	*size = st.st_size;

	return file;
}

uint8_t	*expand_file(uint8_t *file, size_t size, size_t new_size, t_data *data) {
	uint8_t	*new_file;

	new_file = mmap(NULL, new_size, PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new_file == MAP_FAILED) {
		perror("mmap");
		return NULL;
	}
	memset(new_file, 0, new_size);
	memcpy(new_file, file, size);

	munmap(file, size);

	//init_data(data, new_file, new_size);
	update_data(data, new_file, new_size);

	return new_file;
}
