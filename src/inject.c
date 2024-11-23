#include "inject.h"

#define OPTIONS "a:p:o:v"

int	cmp_algo(const char *algo) {
	if (strcmp(algo, "silvio") == 0)
		return SILVIO;
	return -1;
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
		memmove(options->fd_name, argv[1], strlen(argv[1]));
		get_options(argc - 1, argv + 1, options);
		return options;
	}
	return 0;
}

int	main(int argc, char **argv) {
	t_options *options = options_handler(argc, argv);
	if (!options)
		return -1;

	/* get options
	 * handle options (payload and algo)
	 * get payload, put algo in a function pointer
	 * init data
	 * find codecave with the function pointer
	 * inject payload
	 * destroy data
	 */

	return 0;
}
