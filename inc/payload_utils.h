#ifndef PAYLOAD_UTILS_H
# define PAYLOAD_UTILS_H

# include <sys/types.h>
# include <stdint.h>


void modify_payload(int64_t value, size_t offset, size_t size, uint8_t *payload, size_t payload_size);
# endif
