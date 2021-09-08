#ifndef IDX_TYPE_DATA_C_H
#define IDX_TYPE_DATA_C_H

#include <stdlib.h>

enum idx_type {
	IDX_FILE,
	IDX_MEMORY,
};

/** Posible data types */
enum idx_type_data {
	UNSIGNED_8_INT = 0x08,
	SIGNED_8_INT = 0x09,
	SIGNED_16_INT = 0x0B,
	SIGNED_32_INT = 0x0C,
	SIGNED_32_FLOAT = 0x0D,
	SIGNED_64_DOUBLE = 0x0E,
};

size_t idx_type_data_size(enum idx_type_data data);

#endif
