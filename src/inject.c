#include "inject.h"
#include "map.h"
#include "data.h"
#include "payload_utils.h"
#include "options.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <fcntl.h>

#include "silvio.h"
#include "text.h"
#include "bss.h"
#include "note.h"
#include "load.h"
#include "reverse.h"

#define JMP_OFFSET 4

char	g_payload[] = "\x52\xeb\x0f\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x8d\x35\xe0\xff\xff\xff\xba\x0f\x00\x00\x00\x0f\x05\x5a\xe9\xd0\xff\xff\xff";
size_t	g_payload_size	= sizeof(g_payload) - 1;


int	(*g_find_cave[ALGO_SIZE])(t_data *data, size_t payload_size) = {
	&silvio,
	&text,
	&bss,
	&note,
	&load_segment,
	&reverse_text
};

int	patch_new_file(t_data *data) {

	int fd = open("woody", O_CREAT | O_WRONLY | O_TRUNC, 0755);
	if (fd == -1)
		return 1;

	if (write(fd, data->file, data->size) == -1) {
		close(fd);
		return 1;
	}

	close(fd);

	return 0;
}

int	calculate_jmp(t_data *data, size_t payload_size) {

		data->cave.rel_jmp =  \
		(int64_t)data->cave.old_entry - \
		((int64_t)data->cave.addr + (int64_t)payload_size);

	return 0;
}

int	inject(t_data *data, t_options *options) {

	if (g_find_cave[options->algo](data, g_payload_size) == 1) {
		fprintf(stderr, "Error: no cave found\n");
		return 1;
	}

	calculate_jmp(data, g_payload_size);

	modify_payload(data->cave.rel_jmp, JMP_OFFSET, sizeof(data->cave.rel_jmp), (uint8_t *)g_payload, g_payload_size);

	memcpy(data->file + data->cave.offset, g_payload, g_payload_size);

	patch_new_file(data);

	return 0;
}

int	main(int argc, char **argv) {
	t_options *options = options_handler(argc, argv);
	if (!options) {
		dprintf(2, "Usage: %s <binary> -a <algo> -p <payload> -o <output> -v\n", argv[0]);
		return 1;
	}

	size_t size = 0;
	void *file = map_file(options->binary, &size);
	if (!file) {
		return 1;
	}

	t_data data;
	init_data(&data, file, size);

	inject(&data, options);

	free_data(&data);

	return 0;
}
