#ifndef DATA_H
# define DATA_H

# include <elf.h>
# include <sys/types.h>

typedef struct s_elf {
	Elf64_Ehdr	*ehdr;
	Elf64_Shdr	*shdr;
	Elf64_Phdr	*phdr;

	size_t		size;

	//Elf64_Sym	*sym;
	//Elf64_Dyn	*dyn;
	//Elf64_Rel	*rel;
	//Elf64_Rela	*rela;
} t_elf;

typedef struct s_cave {
	Elf64_Addr	addr;
	Elf64_Addr	offset;
	size_t		size;

	Elf64_Addr	old_entry;
	int32_t		rel_jmp;
} t_cave;

typedef struct s_data {
	uint8_t *file;
	size_t	size;

	t_elf	elf;
	t_cave	cave;
} t_data;

int	init_data(t_data *data, uint8_t *file, size_t size);
void	free_data(t_data *data);
void	set_elf_size(t_data *data, size_t size);
int	update_data(t_data *data, uint8_t *file, size_t size);

#endif
