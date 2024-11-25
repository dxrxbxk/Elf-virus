#ifndef SHDR_UTILS_H
# define SHDR_UTILS_H

# include "data.h"

Elf64_Shdr	*get_section_by_off(t_data *data, Elf64_Off offset);
Elf64_Shdr	*get_section_by_name(t_data *data, const char *name);
#endif
