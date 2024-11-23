#include "map.h"

#define BLOCK_SIZE (1 << 12)

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

	*size = (*size > 0) ? *size + st.st_size : st.st_size;

	file = mmap(NULL, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (file == MAP_FAILED) {
		close(fd);
		perror("mmap");
		return NULL;
	}

	uint8_t *ptr = file;
	while ((ret = read(fd, ptr, BLOCK_SIZE)) > 0) {
		ptr += ret;
	}

	if (ret == -1) {
		close(fd);
		munmap(file, *size);
		perror("read");
		return NULL;
	}

	close(fd);

	return file;
}

uint8_t	*expand_file(uint8_t *file, size_t size, size_t new_size) {
	uint8_t	*new_file;

	new_file = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (new_file == MAP_FAILED) {
		perror("mmap");
		return NULL;
	}

	memcpy(new_file, file, size);
	munmap(file, size);
	return new_file;
}
