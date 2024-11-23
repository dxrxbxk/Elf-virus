/* Silvio implementation */
#include "silvio.h"
#include "map.h"

#include <unistd.h>

#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

#include <stdio.h>

int	silvio(t_data *data, size_t payload_size) {
	fprintf(stderr, "Silvio algorithm not implemented\n");

	data->file = expand_file(data->file, data->size, data->size + PAGE_SIZE, data);
	data->size += PAGE_SIZE;


	Elf64_Phdr	*phdr = data->elf.phdr;
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;

	data->cave.old_entry = ehdr->e_entry;

	ehdr->e_shoff += PAGE_SIZE;

	for (int i = 0; i < ehdr->e_phnum; i++) { 
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags == (PF_R | PF_X)) {

			data->cave.offset = phdr[i].p_offset + phdr[i].p_filesz;
			data->cave.addr = phdr[i].p_vaddr + phdr[i].p_filesz;
			
			
			ehdr->e_entry = data->cave.addr;
			

			phdr[i].p_filesz += payload_size;
			phdr[i].p_memsz += payload_size;

			for (int j = i + 1; j < ehdr->e_phnum; j++) {
				phdr[j].p_offset += PAGE_SIZE;
			}

			break;

		}
	}

	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_offset > data->cave.offset) {
			shdr[i].sh_offset += PAGE_SIZE;
		}
		if (shdr[i].sh_addr + shdr[i].sh_size == data->cave.addr) {
			shdr[i].sh_size += payload_size;
		}
	}

	return 0;
}
