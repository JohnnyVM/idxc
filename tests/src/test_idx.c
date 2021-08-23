#include "CppUTest/TestHarness_c.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "idx_c.h"

// dummy function for get the info
void get_bytes(const char* restrict path, uint8_t** data, size_t* len) {
	FILE* fp = fopen(path, "rb");
	if(fp == NULL) {
		return;
	}
	size_t size = BUFSIZ;
	uint8_t buffer[BUFSIZ];

	while(size == BUFSIZ) {
		size = fread(buffer, sizeof **data, BUFSIZ, fp);
		*data = realloc(*data, (*len + size) * sizeof **data);
		memcpy(*data + *len, buffer, size);
		*len += size;
	}
}

TEST_GROUP_C_SETUP(idx)
{
}

TEST_GROUP_C_TEARDOWN(idx)
{
}

/* Read bytes from file and check output is correct */
TEST_C(idx, read_bytes)
{
	struct idx_result out;
	uint8_t* payload = NULL;
	size_t length = 0;

	get_bytes("static/idx/t10k-labels-idx1-ubyte", &payload, &length);

	out = idx_read_bytes(payload, length);
	CHECK_C(!out.error);
	CHECK_C(out.type == IDX_MEMORY);
	struct idx_memory* idx_m = out.memory;
	CHECK_C(idx_m->dimension == 1);
	CHECK_C(idx_m->type_data == UNSIGNED_8_INT);
	CHECK_C(idx_m->number_of_elements == 6000);

	free(payload);
}

