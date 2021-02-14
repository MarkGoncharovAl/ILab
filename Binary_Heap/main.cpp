#include <iostream>
#include "Libraries/Tree.hpp"
#include "Tests/tests.hpp"

constexpr bool RUN_TESTS = true;

int main(int argc, char *argv[])
{
    if (RUN_TESTS)
    {
        testing::InitGoogleTest(&argc, argv);
        return RunTests();
    }
}