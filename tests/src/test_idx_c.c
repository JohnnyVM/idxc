#include "CppUTest/TestHarness_c.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "idx_c.h"

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

	out = idx_memory_from_filename("static/idx/t10k-labels-idx1-ubyte");
	CHECK_C(!out.error && out.type == IDX_MEMORY);
	struct idx_memory* idx_m = out.memory;
	CHECK_C(idx_m->number_of_dimensions == 1);
	CHECK_C(idx_m->type == UNSIGNED_8_INT);
	CHECK_C(idx_m->number_of_elements == 10000);

	struct idx_result res = idx_memory_element(idx_m, 0);
	CHECK_C(!res.error && res.type == IDX_MEMORY && res.memory->type == UNSIGNED_8_INT);
	CHECK_C(*(uint8_t*)res.memory->element == 7);

	idx_result_free(out);
	idx_result_free(res);
}

