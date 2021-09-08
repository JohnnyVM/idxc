#ifndef IDX_MEMORY_C_H
#define IDX_MEMORY_C_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "idx_type_data_c.h"
#include "idx_error_c.h"

/** Basic struct for contain the data */
struct idx_memory {
	uint16_t must_0; /**< First 2 bytes are 0 */
	uint8_t type; /**< type data */
	uint8_t number_of_dimensions; /**< number of dimensions */
	uint32_t number_of_elements; /**< number of elements in the file */
	size_t element_size;
	uint32_t* dimension; /**< dimension length, the array have number_of_dimensions length */
	uint8_t element[]; /**< data array in "brute", require process for recover data */
};

struct idx_result idx_memory_from_bytes(uint8_t* bytes, size_t length);
struct idx_result idx_memory_from_filename(const char* filename);
struct idx_result idx_memory_from_file(FILE* fp);
size_t idx_memory_header_size(struct idx_memory* memory);
struct idx_result idx_memory_element(struct idx_memory* elem, size_t position);
size_t idx_memory_element_size(struct idx_memory* memory);
struct idx_result idx_memory_slice(struct idx_memory* memory, size_t initial_position, size_t final_position);

void idx_memory_free(struct idx_memory* elem);

#endif

