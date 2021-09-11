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


/* return the size of a element */
size_t idx_memory_element_size(struct idx_memory* mem) {
	size_t sum = 1;

	for(unsigned i = 0; mem->number_of_dimensions > 1 && i < mem->number_of_dimensions;i++) {
		sum *= mem->dimension[i];
	}

	sum *= idx_type_data_size(mem->type);

	return sum;
}

/** Return the number of bytes of the data that define the document(header?) */
size_t idx_memory_header_size(struct idx_memory* memory) {
	return  4 + sizeof(uint32_t) * (memory->number_of_dimensions == 1 ? 1: memory->number_of_dimensions + 1); // define dimension length
}

void idx_memory_free(struct idx_memory* memory) {
	if(memory == NULL) { return; }
	if(memory->number_of_dimensions > 0) {
		free(memory->dimension);
	}
	free(memory);
}

struct idx_result idx_memory_from_filename(const char* filename) {
	struct idx_result output = {0};

	if(filename == NULL) {
		output.error = INVALID_VALUES;
		return output;
	}

	FILE* fp = fopen(filename, "rb");
	if(fp == NULL) {
		output.error = INVALID_VALUES;
		return output;
	}

	output = idx_memory_from_file(fp);

	fclose(fp);

	return output;
}

struct idx_result idx_memory_from_file(FILE* fp) {
	size_t size = BUFSIZ, len = 0;
	uint8_t buffer[BUFSIZ], *data = NULL;
	struct idx_result output = {0};

	if(fp == NULL) {
		output.error = INVALID_VALUES;
		return output;
	}

	while(size == BUFSIZ) {
		size = fread(buffer, sizeof *data, BUFSIZ, fp);
		data = realloc(data, (len + size) * sizeof *data);
		memcpy(data + len, buffer, size);
		len += size; // man not said fread can return less than 0
	}

	output = idx_memory_from_bytes(data, len);

	free(data);

	return output;
}

struct idx_result idx_memory_from_bytes(uint8_t* bytes, size_t length) {
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

	idx_m->type = bytes[2];
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

	idx_m->number_of_elements = be32toh(*(uint32_t*)(&bytes[4]));
	// Number of elements in each dimension
	idx_m->dimension = malloc(idx_m->number_of_dimensions * sizeof *idx_m->dimension);
	if(idx_m->dimension == NULL) {
		output.error = NOT_ENOUGHT_MEMORY;
		return output;
	}
	if(idx_m->number_of_dimensions == 1) {
		idx_m->dimension[0] = idx_m->number_of_elements;
	}
	for(size_t i = 0;idx_m->number_of_dimensions > 1 && i < idx_m->number_of_dimensions; i++) {
		idx_m->dimension[i] = be32toh(((uint32_t*)&bytes[8])[i]);
	}

	/// \todo check the size is coherent
	idx_m->element_size = idx_memory_element_size(idx_m);

	memcpy(idx_m->element, &bytes[idx_memory_header_size(idx_m)], length - idx_memory_header_size(idx_m)); // data

	output.type = IDX_MEMORY;
	output.memory = idx_m;
	return output;
}

struct idx_result idx_memory_slice(struct idx_memory* memory, size_t initial_position, size_t final_position) {
	struct idx_result output = {0};
	// copy the headers
	if(memory == NULL
		|| initial_position > final_position
		|| final_position >= memory->number_of_elements) {

		output.error = INVALID_VALUES;
		return output;
	}

	output.type = IDX_MEMORY;

	// Memory have to be allocated in block
	output.memory = malloc(
			sizeof *output.memory +
			(uint32_t)(final_position - initial_position) * memory->element_size
	);
	if(!output.memory) {
		output.error = NOT_ENOUGHT_MEMORY;
		return output;
	}
	*output.memory = *memory;
	output.memory->number_of_elements = (uint32_t)(final_position - initial_position);

	memcpy(
			output.memory->element,
			memory->element
			+ memory->element_size * initial_position,
			output.memory->number_of_elements * memory->element_size
	);

	// Memory for dimensions
	output.memory->dimension = malloc(
			output.memory->number_of_dimensions * sizeof *output.memory->dimension
	);
	memcpy(
			output.memory->dimension,
			memory->dimension,
			output.memory->number_of_dimensions * sizeof *output.memory->dimension
	);

	return output;
}

struct idx_result idx_memory_element(struct idx_memory* mem, size_t position) {
	return idx_memory_slice(mem, position, position + 1);
}
