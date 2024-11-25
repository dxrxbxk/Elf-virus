#include "shdr_utils.h"
#include <string.h>
#include <stdio.h>


Elf64_Shdr	*get_section_by_name(t_data *data, const char *name) {
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;

	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (strcmp((char *)data->file + shdr[i].sh_name, name) == 0) {
			return &shdr[i];
		}
	}
	return NULL;
}

Elf64_Shdr	*get_section_by_addr(t_data *data, Elf64_Addr addr) {
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;

	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_addr == addr) {
			return &shdr[i];
		}
	}
	return NULL;
}

Elf64_Shdr	*get_section_by_off(t_data *data, Elf64_Off offset) {
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;
	Elf64_Shdr	*strtab = &shdr[ehdr->e_shstrndx];
	char		*strtab_p = (char *)data->file + strtab->sh_offset;

	for (int i = 0; i < ehdr->e_shnum; i++) {
		if (shdr[i].sh_offset == offset) {

			printf("name: %s\n", strtab_p + shdr[i].sh_name);
			return &shdr[i];
		}
	}
	return NULL;
}
