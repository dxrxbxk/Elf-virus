#include "inject.h"
#include "map.h"
#include "data.h"
#include "silvio.h"
#include "text.h"
#include "payload_utils.h"

#define OPTIONS "a:p:o:v"

#define JMP_OFFSET 4
#define DEFAULT_ALGO TEXT

char	g_payload[] = "\x52\xeb\x0f\x2e\x2e\x2e\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x2e\x0a\x00\xb8\x01\x00\x00\x00\xbf\x01\x00\x00\x00\x48\x8d\x35\xe0\xff\xff\xff\xba\x0f\x00\x00\x00\x0f\x05\x5a\xe9\xd0\xff\xff\xff";
size_t	g_payload_size	= sizeof(g_payload) - 1;

int	cmp_algo(const char *algo) {
	if (strcmp(algo, "silvio") == 0)
		return SILVIO;
	else if (strcmp(algo, "text") == 0)
		return TEXT;
	return DEFAULT_ALGO;
}

int get_options(int argc, char **argv, t_options *options) {
	int opt = 0;

	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
		switch (opt) {
			case 'a':
				options->algo = cmp_algo(optarg);
				if (options->algo == -1)
					return -1;

				break;

			case 'p':
				break;

			case 'v':
				break;

			case 'o':
				break;

			case '?':
				return -1;

			default:
				return -1;
		}
	}
	return 0;
}

t_options	*init_options(void) {
	static t_options options;
	memset(&options, 0, sizeof(t_options));
	return &options;
}

t_options *options_handler(int argc, char **argv) {
	if (argc < 2)
		return NULL;
	else if (argc > 1) {
		t_options *options = init_options();
		options->binary = argv[1];
		if (argc > 2) {
			get_options(argc - 1, argv + 1, options);
			return options;
		}
		else {
			options->algo = DEFAULT_ALGO;
			return options;
		}
	}
	return 0;
}

int	(*g_find_cave[ALGO_SIZE])(t_data *data, size_t payload_size) = {
	&silvio,
	&text,
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

int	inject(t_data *data, t_options *options) {

	if (g_find_cave[options->algo](data, g_payload_size) == 1) {
		fprintf(stderr, "Error: no cave found\n");
		return 1;
	}

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
