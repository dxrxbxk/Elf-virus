/* Data segment implementation */

#include "bss.h"
#include "map.h"
#include "shdr_utils.h"
#include <string.h>

#include <stdio.h>

#define ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))


void iterate_symtab(t_data *data) {
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)data->file;
    Elf64_Shdr *section_headers = (Elf64_Shdr *)((char *)elf_header + elf_header->e_shoff);

	// find the section header string table
    for (int i = 0; i < elf_header->e_shnum; i++) {
        if (section_headers[i].sh_type == SHT_SYMTAB) {
            Elf64_Sym *symtab = (Elf64_Sym *)((char *)elf_header + section_headers[i].sh_offset);
            int sym_count = section_headers[i].sh_size / section_headers[i].sh_entsize;
            const char *strtab = (char *)elf_header + section_headers[section_headers[i].sh_link].sh_offset;

            for (int j = 0; j < sym_count; j++) {
                const char *name = strtab + symtab[j].st_name;
                printf("Symbol %d: Name = %s, Value = 0x%lx, Size = %lu, Info = 0x%x\n",
                       j, name, symtab[j].st_value, symtab[j].st_size, symtab[j].st_info);
            }
        }
    }
}

void pad_symtab(t_data *data, size_t offset, size_t payload_size) {
	Elf64_Ehdr *elf_header = (Elf64_Ehdr *)data->file;
	Elf64_Shdr *section_headers = (Elf64_Shdr *)((char *)elf_header + elf_header->e_shoff);

	for (size_t i = 0; i < elf_header->e_shnum; ++i) {
		if (section_headers[i].sh_type != SHT_SYMTAB) {
			continue;
		}
		Elf64_Sym *symtab = (Elf64_Sym *)((char *)elf_header + section_headers[i].sh_offset);
		int sym_count = section_headers[i].sh_size / section_headers[i].sh_entsize;
		const char *strtab = (char *)elf_header + section_headers[section_headers[i].sh_link].sh_offset;

		for (int j = 0; j < sym_count; j++) {
			if (symtab[j].st_value >= offset) {
				symtab[j].st_value += payload_size;
			}
		}
	}
}


int	bss(t_data *data, size_t payload_size) {




	printf("payload_size: %lu\n", payload_size);

	size_t elf_size = data->size;
	data->file = expand_file(data->file, \
			data->size, data->size + payload_size, data);
	if (!data->file)
		return -1;

	Elf64_Phdr	*phdr = data->elf.phdr;
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;

	data->cave.old_entry = ehdr->e_entry;


	//iterate_symtab(data);
	//ehdr->e_shoff += payload_size;

	//ehdr->e_phoff += payload_size; 
	//ehdr->e_shoff += payload_size;

	ehdr->e_shoff += payload_size;

	size_t bss_len;

	for (size_t i = ehdr->e_phnum; i--;) {
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags == (PF_R | PF_W)) {

			data->cave.offset = phdr[i].p_offset + phdr[i].p_filesz;
			data->cave.addr = phdr[i].p_vaddr + phdr[i].p_filesz;

			bss_len = phdr[i].p_memsz - phdr[i].p_filesz;
			//pad_symtab(data, data->cave.offset, payload_size);
			//data->cave.offset += bss_len;
			payload_size += bss_len;


			ehdr->e_entry = data->cave.addr + bss_len;

			phdr[i].p_filesz += payload_size;
			phdr[i].p_memsz += payload_size;

			phdr[i].p_flags |= PF_X;

			printf("data->cave.offset: %lx\n", data->cave.offset);

			break;

		}

	}


	for (size_t i = ehdr->e_shnum; i--;) {
		if (shdr[i].sh_addr >= data->cave.addr) {
			shdr[i].sh_addr += payload_size;
		}
		if (shdr[i].sh_offset >= data->cave.offset) {
			shdr[i].sh_offset += payload_size;
		}
		else if (shdr[i].sh_addr + shdr[i].sh_size == data->cave.addr) {
			shdr[i].sh_size += payload_size;
		}
	}



	memmove(data->file + data->cave.offset + payload_size, data->file + data->cave.offset, elf_size - data->cave.offset);
	memset(data->file + data->cave.offset, 0, payload_size);
	data->cave.offset += bss_len;
	data->cave.addr += bss_len;

	return 0;
}
