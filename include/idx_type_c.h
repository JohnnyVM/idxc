#ifndef IDX_TYPE_C_H
#define IDX_TYPE_C_H

enum idx_type {
	IDX_FILE,
	IDX_MEMORY,
	IDX_ELEMENT,
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


#endif
