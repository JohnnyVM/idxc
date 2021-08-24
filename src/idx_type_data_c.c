#include <stdlib.h>
#include <stdint.h>

#include "idx_type_data_c.h"

size_t idx_type_data_size(enum idx_type_data data) {
	switch(data) {
		case UNSIGNED_8_INT:
			return sizeof(uint8_t);
		case SIGNED_8_INT:
			return sizeof(int8_t);
		case SIGNED_16_INT:
			return sizeof(int16_t);
		case SIGNED_32_INT:
			return sizeof(int32_t);
		case SIGNED_32_FLOAT:
			return sizeof(float);
		case SIGNED_64_DOUBLE:
			return sizeof(double);
	}
	return 0;
}
