#ifndef IDX_ELEMENT_C_H
#define IDX_ELEMENT_C_H

#include <stdint.h>

#include "idx_type_c.h"

#define define_idx_element(type)\
struct idx_element_##type {\
	uint8_t number_of_dimensions;\
	uint32_t* dimension_length;\
	type element[];\
}

define_idx_element(uint8_t);

define_idx_element(int8_t);

define_idx_element(int16_t);

define_idx_element(int32_t);

define_idx_element(float);

define_idx_element(double);


struct idx_element {
	enum idx_type_data type;
	union {
		struct idx_element_uint8_t uint8t;
		struct idx_element_int8_t int8t;
		struct idx_element_int16_t int16t;
		struct idx_element_int32_t int32t;
		struct idx_element_float floatt;
		struct idx_element_double doublet;
	};
};

#endif

