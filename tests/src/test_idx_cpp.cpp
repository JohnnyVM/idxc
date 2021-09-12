#include "CppUTest/TestHarness.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "idx.hpp"

TEST_GROUP(idx_cpp)
{
};

TEST(idx_cpp, check_constructor)
{

	auto out = Idx("static/idx/t10k-labels-idx1-ubyte");
	CHECK(out.type == UNSIGNED_8_INT);
	CHECK(out.number_of_elements == 10000);

	auto slice_2 = out.slice(4,9);
	CHECK(slice_2.number_of_elements == 5);
	auto slice = out[0];

	uint8_t check[] = {4,1,4,9,5};
	for(size_t i = 0; i < slice_2.number_of_elements; i++) {
		CHECK((static_cast<uint8_t*>(slice_2))[i] == check[i]);
	}

	CHECK(*static_cast<uint8_t*>(slice) == 7);
}

