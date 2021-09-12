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

TEST(idx_cpp, check_constructor_images)
{

	auto out = Idx("static/idx/t10k-images.idx3-ubyte");
	CHECK(out.type == UNSIGNED_8_INT);
	CHECK(out.number_of_elements == 10000);
	CHECK(out.dimension.size() == 2);
	CHECK(out.dimension[0] == 28);
	CHECK(out.dimension[1] == 28);

	auto slice = out[0];
	CHECK(slice.number_of_elements == 1);

	uint8_t check[] = {0,0,0,0,0}; // The beginning of the iamge is 0
	for(size_t i = 0; i < 5; i++) {
		CHECK((static_cast<uint8_t*>(slice.payload.get()))[i] == check[i] );
	}
}
