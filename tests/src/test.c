#include "CppUTest/TestHarness_c.h"

TEST_GROUP_C_SETUP(template_name)
{
}

TEST_GROUP_C_TEARDOWN(template_name)
{
}

TEST_C(template_name, dummy_test)
{
	CHECK_C(1==1);
}

