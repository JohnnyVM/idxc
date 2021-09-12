#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

/** For each C test group */
TEST_GROUP_C_WRAPPER(idx)
{
    TEST_GROUP_C_SETUP_WRAPPER(idx); /** optional */
    TEST_GROUP_C_TEARDOWN_WRAPPER(idx); /** optional */
};

TEST_C_WRAPPER(idx, read_bytes);
TEST_C_WRAPPER(idx, read_images);

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
