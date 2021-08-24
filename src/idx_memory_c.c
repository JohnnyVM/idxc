#include <endian.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "idx_memory_c.h"
#include "idx_result_c.h"

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
size_t idx_memory_header_bytes(struct idx_memory* memory) {
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
		if(idx_m->dimension_length == NULL) {
			output.error = NOT_ENOUGHT_MEMORY;
			return output;
		}
		for(size_t i = 0; i < idx_m->number_of_dimensions; i++) {
			idx_m->dimension_length[i] = be32toh(((uint32_t*)&bytes[8])[i]);
		}
	}

	/// \todo check the size is coherent

	memcpy(idx_m->element, &bytes[idx_memory_header_bytes(idx_m)], length - idx_memory_header_bytes(idx_m)); // data

	output.type = IDX_MEMORY;
	output.memory = idx_m;
	return output;
}

struct idx_result idx_memory_element(struct idx_memory* memory, size_t position) {
	struct idx_result output = {0};
	// copy the headers
	if(memory == NULL || position >= memory->number_of_elements) {
		output.error = INVALID_VALUES;
		return output;
	}

	output.type = IDX_ELEMENT;

	struct idx_element *element = output.element = malloc(sizeof *output.element);
	if(element == NULL) {
		output.error = NOT_ENOUGHT_MEMORY;
		return output;
	}

	element->type = memory->type;
	element->number_of_dimensions = memory->number_of_dimensions;
	element->dimension_length = NULL;
	if(element->number_of_dimensions > 1) {
		element->dimension_length = malloc(element->number_of_dimensions * sizeof(uint32_t));
		if(element->dimension_length == NULL) {
			free(element);
			output.error = NOT_ENOUGHT_MEMORY;
			return output;
		}
		for(size_t i = 0; i < element->number_of_dimensions; i++) {
			element->dimension_length[i] = memory->dimension_length[i];
		}
	}

	element->value = malloc(idx_element_value_size(element));
	if(element->dimension_length == NULL) {
		free(element->dimension_length);
		free(element);
		output.error = NOT_ENOUGHT_MEMORY;
		return output;
	}

	memcpy(
			element->value,
			&((uint8_t*)memory->element)[position * idx_element_value_size(element)],
			idx_element_value_size(element)
	);

	return output;
}

