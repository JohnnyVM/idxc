#ifndef IDX_C_H
#define IDX_C_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/** Posible data types */
enum idx_type_data {
	UNSIGNED_8_INT = 0x08,
	SIGNED_8_INT = 0x09,
	SIGNED_16_INT = 0x0B,
	SIGNED_32_INT = 0x0C,
	SIGNED_32_FLOAT = 0x0D,
	SIGNED_64_DOUBLE = 0x0E,
};

enum idx_type {
	IDX_FILE,
	IDX_MEMORY,
};

/** Basic struct for contain the data */
struct idx_memory {
	uint16_t must_0; /**< First 2 bytes are 0 */
	uint8_t type_data; /**< type data */
	uint8_t number_of_dimensions; /**< number of dimensions */
	uint32_t* dimension_length; /**< dimension length, the array have number_of_dimensions length */
	uint8_t element[]; /**< data array in "brute", require process for recover data */
};

struct idx_file {
	uint16_t must_0; /**< First 2 bytes are 0 */
	uint8_t type_data; /**< type data */
	uint8_t number_of_dimensions; /**< number of dimensions */
	uint32_t* dimension_length;
	FILE* fp; /**< if the data is not saved in memory current position of the file cursor */
};

/** List of posible errors */
enum idx_type_error {
	NO_ERROR = 0,
	INVALID_VALUES, /**< unespected values passed to function */
	INVALID_FILE, /**< After parse the document invalid values encountred */
	NOT_ENOUGHT_MEMORY, /**< malloc or similar returned null */
};

struct idx_error_message {
	size_t length;
	char message[];
};

/** Operation output container */
struct idx_result {
	enum idx_type_error error;
	enum idx_type type;
	union {
		struct idx_error_message* error_message;
		struct idx_memory* memory;
		struct idx_file* file;
	};
};

struct idx_result idx_read_bytes(uint8_t* bytes, size_t length);
size_t idx_memory_length(struct idx_memory* memory);

#endif

