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
}

