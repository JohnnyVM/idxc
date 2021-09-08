#ifndef IDX_COMMON_C_H
#define IDX_COMMON_C_H

#include "idx_memory_c.h"
#include "idx_file_c.h"

#define idx_element_size(X) \
	_Generix(X, \
		struct idx_memory*: idx_memory_element_size \
	)(X)

#define idx_header_size(X) \
	_Generix(X, \
		struct idx_memory*: idx_memory_header_size \
	)(X)

#endif
