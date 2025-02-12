/* Data segment implementation */

#include "bss.h"
#include "map.h"
#include "shdr_utils.h"
#include <string.h>

#define PAGE_SIZE 0x1000

#include <stdio.h>

int	bss(t_data *data, size_t payload_size) {


	size_t elf_size = data->size;
	data->file = expand_file(data->file, \
			data->size, data->size + payload_size, data);
	if (!data->file)
		return -1;

	Elf64_Phdr	*phdr = data->elf.phdr;
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;
	Elf64_Addr bss_offset = 0;

	data->cave.old_entry = ehdr->e_entry;

	ehdr->e_shoff += payload_size;


	for (int i = 0; i < ehdr->e_phnum; i++) { 
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags == (PF_R | PF_W)) {

			data->cave.offset = phdr[i].p_offset + phdr[i].p_filesz;
			data->cave.addr = phdr[i].p_vaddr + phdr[i].p_filesz;

			//printf("data->cave.offset: %lx\n", data->cave.offset);

			bss_offset = phdr[i].p_offset + phdr[i].p_memsz;


			ehdr->e_entry = data->cave.addr;

			phdr[i].p_filesz += payload_size;
			phdr[i].p_memsz += payload_size;

			phdr[i].p_flags |= PF_X;

			printf("data->cave.offset: %lx\n", data->cave.offset);

			break;

		}

	}

	Elf64_Shdr	*shdr_bss = get_section_by_off(data, data->cave.offset);
	if (!shdr_bss) {
		fprintf(stderr, "Error: no .bss section found\n");
	}
	shdr_bss->sh_offset += payload_size;
	shdr_bss->sh_addr += payload_size;
	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_offset > data->cave.offset) {
			shdr[i].sh_offset += payload_size;
		}
		else if (shdr[i].sh_addr + shdr[i].sh_size == data->cave.addr) {
			shdr[i].sh_size += payload_size;
		}
	}



	memmove(data->file + data->cave.offset + payload_size, data->file + data->cave.offset, elf_size - data->cave.offset);
	memset(data->file + data->cave.offset, 0, payload_size);
	return 0;
}
