#ifndef IDX_RESULT_C_H
#define IDX_RESULT_C_H

#include "idx_type_data_c.h"
#include "idx_error_c.h"

/** Operation output container */
struct idx_result {
	enum idx_type_error error;
	enum idx_type type;
	union {
		struct idx_error_message* error_message;
		struct idx_memory* memory;
		struct idx_file* file;
		struct idx_element* element;
	};
};

void idx_result_free(struct idx_result result);

#endif
