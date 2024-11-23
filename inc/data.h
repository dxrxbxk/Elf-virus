#ifndef DATA_H
# define DATA_H

# include <elf.h>
# include <sys/types.h>

typedef struct s_elf {
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr	*shdr;
	Elf64_Sym	*sym;
	Elf64_Rel	*rel;
	Elf64_Rela	*rela;
	Elf64_Phdr	*phdr;
	Elf64_Dyn	*dyn;
} t_elf;

typedef struct s_data {
	uint8_t *file;
	size_t	size;

	t_elf	elf;
} t_data;

#endif
