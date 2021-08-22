#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness_c.h"

/** For each C test group */
TEST_GROUP_C_WRAPPER(template_name)
{
    TEST_GROUP_C_SETUP_WRAPPER(template_name); /** optional */
    TEST_GROUP_C_TEARDOWN_WRAPPER(template_name); /** optional */
};

TEST_C_WRAPPER(template_name, dummy_test);

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
