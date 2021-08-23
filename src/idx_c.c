#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <endian.h>

#include "idx_c.h"

static bool check_idx_type_data(enum idx_type_data tdata) {
	switch(tdata) {
		case UNSIGNED_8_INT:
		case SIGNED_8_INT:
		case SIGNED_16_INT:
		case SIGNED_32_INT:
		case SIGNED_32_FLOAT:
		case SIGNED_64_DOUBLE:
			return true;
		default:
			return false;
	}
}

struct idx_result idx_read_bytes(uint8_t* bytes, size_t length) {
	struct idx_result output = {0};

	if(bytes == NULL || length < 8 ) {
		output.error = INVALID_VALUES;
		return output;
	}

	struct idx_memory* idx_m = malloc(sizeof(struct idx_memory) + (length - 8)); // remove the 8 first bytes
	if(idx_m == NULL) {
		output.error = NOT_ENOUGHT_MEMORY;
		return output;
	}

	/** Copy the members of the struct */
	memcpy(&idx_m->must_0, bytes, 2);
	if(idx_m->must_0 != 0) {
		free(idx_m);
		output.error = INVALID_FILE;
		return output;
	}

	memcpy(&idx_m->type_data, &bytes[2], 1); // check type
	if(!check_idx_type_data(idx_m->type_data)) {
		free(idx_m);
		output.error = INVALID_FILE;
		return output;
	}

	memcpy(&idx_m->dimension, &bytes[3], 1); // dimension

	uint32_t little_endian_32bits;
	memcpy(&little_endian_32bits, &bytes[4], 4); // dimension
	idx_m->number_of_elements = le32toh(little_endian_32bits);

	memcpy(idx_m->element, &bytes[8], length - 8); // data

	output.type = IDX_MEMORY;
	output.memory = idx_m;
	return output;
}
