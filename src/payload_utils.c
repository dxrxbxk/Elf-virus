#include "payload_utils.h"

void modify_payload(int64_t value, size_t offset, size_t size, uint8_t *payload, size_t payload_size) {

	for (size_t i = size; i > 0; i--) {
		payload[payload_size - offset] = value & 0xFF;
		value >>= 8;
		offset--;
	}
}
