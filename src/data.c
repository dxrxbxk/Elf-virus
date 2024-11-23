#include "data.h"
#include <sys/mman.h>

int	init_data(t_data *data, uint8_t *file, size_t size) {
	memset(data, 0, sizeof(t_data));
	data->file = file;
	data->size = size;

	data->elf.ehdr = (Elf64_Ehdr *)file;
	data->elf.phdr = (Elf64_Phdr *)(file + data->elf.ehdr->e_phoff);
	data->elf.shdr = (Elf64_Shdr *)(file + data->elf.ehdr->e_shoff);

	return 0;

}

void	free_data(t_data *data) {
	if (data->file)
		munmap(data->file, data->size);
}
