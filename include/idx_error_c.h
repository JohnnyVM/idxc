#ifndef IDX_ERROR_C_H
#define IDX_ERROR_C_H

#include <stdio.h>

/** List of posible errors */
enum idx_type_error {
	NO_ERROR = 0,
	INVALID_VALUES, /**< unespected values passed to function */
	INVALID_FILE, /**< After parse the document invalid values encountred */
	NOT_ENOUGHT_MEMORY, /**< malloc or similar returned null */
	OUT_OF_BOUND, /**< usually out of index number */
};

struct idx_error_message {
	size_t length;
	char message[];
};

#endif
