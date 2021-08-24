#ifndef IDX_ELEMENT_C_H
#define IDX_ELEMENT_C_H

#include <stdint.h>

#include "idx_type_data_c.h"

struct idx_element {
	enum idx_type_data type;
	uint8_t number_of_dimensions;
	uint32_t* dimension_length;
	void* value;
};

size_t idx_element_value_size(struct idx_element* element);
void idx_element_free(struct idx_element* element);

#endif

