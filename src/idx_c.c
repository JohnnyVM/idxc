#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "idx_c.h"

#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))

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

static size_t idx_type_data_size(enum idx_type_data tdata) {
	switch(tdata) {
		case UNSIGNED_8_INT:
			return sizeof(uint8_t);
		case SIGNED_8_INT:
			return sizeof(int8_t);
		case SIGNED_16_INT:
			return sizeof(int16_t);
		case SIGNED_32_INT:
			return sizeof(int32_t);
		case SIGNED_32_FLOAT:
			return sizeof(float);
		case SIGNED_64_DOUBLE:
			return sizeof(double);
		default:
			return 0U;
	}
}

/** Return the number of bytes of the data that define the document(header?) */
static size_t idx_memory_header_size(struct idx_memory* memory) {
	return  4 + sizeof(uint32_t) * memory->number_of_dimensions; // define dimension length
}

/** Return the number of elements that contain the file */
size_t idx_memory_length(struct idx_memory* memory) {
	if(memory == NULL) { return 0; }
	size_t sum = 1;
	for(size_t i = 0; i < memory->number_of_dimensions; i++) {
		sum *= memory->dimension_length[i];
	}
	return sum;
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

	memcpy(&idx_m->number_of_dimensions, &bytes[3], 1); // dimension
	if(!idx_m->number_of_dimensions) {
		free(idx_m);
		output.error = INVALID_FILE;
		return output;
	}

	// Number of elements in each dimension
	idx_m->dimension_length = malloc(idx_m->number_of_dimensions * sizeof(uint32_t));
	for(size_t i = 0; i < idx_m->number_of_dimensions; i++) {
		idx_m->dimension_length[i] = SWAP_UINT32(((uint32_t*)&bytes[4])[i]);
	}

	// Checkpoint check the size is coherent
	size_t sum = idx_memory_length(idx_m); // number of elements
	sum *= idx_type_data_size(idx_m->type_data);
	sum += idx_memory_header_size(idx_m);
	if(length != sum) {
		free(idx_m);
		output.error = INVALID_FILE;
		return output;
	}

	memcpy(idx_m->element, &bytes[idx_memory_header_size(idx_m)], length - idx_memory_header_size(idx_m)); // data

	output.type = IDX_MEMORY;
	output.memory = idx_m;
	return output;
}
