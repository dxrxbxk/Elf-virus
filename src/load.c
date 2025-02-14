#include "load.h"
#include <string.h>
#include <unistd.h>

#include "map.h"
/*create a new segment in the elf file*/

#include <stdio.h>

#define PAGE_SIZE 0x1000

int load_segment(t_data *data, size_t payload_size) {

	//size_t new_offset = data->elf.size + payload_size;
	
	size_t elf_size = data->size;
	data->file = expand_file(data->file, data->size, data->size + PAGE_SIZE, data);
	if (!data->file)
		return -1;

	Elf64_Phdr *phdr = data->elf.phdr;
	Elf64_Ehdr *ehdr = data->elf.ehdr;
	Elf64_Shdr *shdr = data->elf.shdr;

	data->cave.offset = (elf_size + PAGE_SIZE) & ~(PAGE_SIZE - 1);
	data->cave.addr = phdr[ehdr->e_phnum - 1].p_vaddr + phdr[ehdr->e_phnum - 1].p_memsz;


	Elf64_Phdr new_phdr = {
		.p_type = PT_LOAD,
		.p_flags = PF_R | PF_X,
		.p_offset = data->cave.offset,
		.p_vaddr = data->cave.addr,
		.p_paddr = data->cave.addr,
		.p_filesz = payload_size,
		.p_memsz = payload_size,
		.p_align = 0x1000
	};


	ehdr->e_phnum++;

	data->cave.old_entry = ehdr->e_entry;
	ehdr->e_entry = data->cave.addr;

	memcpy(phdr + ehdr->e_phnum - 1, &new_phdr, sizeof(Elf64_Phdr));

	return 0;
}
