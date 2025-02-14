/* Text segment implementation */

#include "text.h"

#define PAGE_SIZE	sysconf(_SC_PAGE_SIZE)

int	text(t_data *data, size_t payload_size) {

	Elf64_Ehdr	*ehdr	= (Elf64_Ehdr *)data->file;
	Elf64_Phdr	*phdr	= (Elf64_Phdr *)(data->file + ehdr->e_phoff);

	for (size_t i = 0; i < ehdr->e_phnum; ++i) {

		if (phdr[i].p_type != PT_LOAD || (phdr[i].p_flags & PF_X) == 0)
			continue;

		data->cave.offset = phdr[i].p_offset + phdr[i].p_filesz;
		data->cave.addr = phdr[i].p_vaddr + phdr[i].p_filesz;
		int next_offset = phdr[i].p_offset + (phdr[i].p_memsz / PAGE_SIZE + 1) * PAGE_SIZE;
		data->cave.size = next_offset - data->cave.offset;

		if (payload_size > data->cave.size)
			return 1;

		data->cave.old_entry = ehdr->e_entry;
		ehdr->e_entry     = phdr[i].p_vaddr + phdr[i].p_filesz;
		phdr[i].p_flags  |= PF_W;
		phdr[i].p_filesz += payload_size;
		phdr[i].p_memsz  += payload_size;

		return 0;

	}

	return 1;
}
