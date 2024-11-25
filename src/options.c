#include "options.h"
#include "inject.h"

int	cmp_algo(const char *algo) {
	if (strcmp(algo, "silvio") == 0)
		return SILVIO;
	else if (strcmp(algo, "text") == 0)
		return TEXT;
	else if (strcmp(algo, "bss") == 0)
		return BSS;
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
	return NULL;
}
