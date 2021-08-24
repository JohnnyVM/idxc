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

/** Return the number of bytes of the data that define the document(header?) */
static size_t idx_memory_header_bytes(struct idx_memory* memory) {
	return  4 + sizeof(uint32_t) * (memory->number_of_dimensions == 1 ? 1: memory->number_of_dimensions + 1); // define dimension length
}

void idx_memory_free(struct idx_memory* memory) {
	if(memory == NULL) { return; }
	if(memory->number_of_dimensions > 1) {
		free(memory->dimension_length);
	}
	free(memory);
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

	memcpy(&idx_m->type, &bytes[2], 1); // check type
	if(!check_idx_type_data(idx_m->type)) {
		free(idx_m);
		output.error = INVALID_FILE;
		return output;
	}

	memcpy(&idx_m->number_of_dimensions, &bytes[3], 1); // dimension
	if(!idx_m->number_of_dimensions) {
		free(idx_m);
		output.error = INVALID_FILE;
		return output;
	}

	/** I dont understand what happen here, very probably this work accidentally */
	idx_m->number_of_elements = be32toh(*(uint32_t*)(&bytes[4]));

	// Number of elements in each dimension
	if(idx_m->number_of_dimensions > 1) {
		idx_m->dimension_length = malloc(idx_m->number_of_dimensions * sizeof(uint32_t));
		for(size_t i = 0; i < idx_m->number_of_dimensions; i++) {
			idx_m->dimension_length[i] = (((uint32_t*)&bytes[8])[i]);
		}
	}

	/// \todo check the size is coherent

	memcpy(idx_m->element, &bytes[idx_memory_header_bytes(idx_m)], length - idx_memory_header_bytes(idx_m)); // data

	output.type = IDX_MEMORY;
	output.memory = idx_m;
	return output;
}

