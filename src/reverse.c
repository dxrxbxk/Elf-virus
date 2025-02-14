/* Reverse text infection */

#include "silvio.h"
#include "map.h"
#include <string.h>
#include <unistd.h>

#include <stdio.h>


#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)
#define PAGE_ROUNDUP(x) ((x + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

int reverse_text(t_data *data, size_t payload_size) {
	Elf64_Phdr	*phdr = data->elf.phdr;
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;

	printf("sizeof(*ehdr) = %lu\n", sizeof(*ehdr));

	return 0;
}
