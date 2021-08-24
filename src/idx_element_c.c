#include <stdlib.h>

#include "idx_type_data_c.h"
#include "idx_element_c.h"

/** \brief return the number of bytes of a element */
size_t idx_element_value_size(struct idx_element* element) {
	size_t sum = 1;
	if(element->number_of_dimensions > 1) {
		for(size_t i = 0; i < element->number_of_dimensions; i++) {
			sum *= element->dimension_length[i];
		}
	}
	sum *= idx_type_data_size(element->type);

	return sum;
}
