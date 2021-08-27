#ifndef IDX_FILE_C_H
#define IDX_FILE_C_H

#include <stdio.h>
#include <stdint.h>

struct idx_file {
	uint16_t must_0; /**< First 2 bytes are 0 */
	uint8_t type; /**< type data */
	uint8_t number_of_dimensions; /**< number of dimensions */
	uint32_t number_of_elements; /**< number of elements in the file */
	uint32_t* dimension;
	FILE* fp; /**< if the data is not saved in memory current position of the file cursor */
};

#endif

