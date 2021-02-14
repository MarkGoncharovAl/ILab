#include <iostream>
#include <gtest/gtest.h>
#include "BSort/BSort.hpp"
#include "Common_libs/Random/Random.hpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(BSort, easy_incr)
{
    std::vector<int> test = {1, 3, 2, 6, 5};
    std::vector<int> copy_test = test;

    clM::BSort(test, clM::Sort::Incr);
    std::sort(copy_test.begin(), copy_test.end());

    ASSERT_EQ(test, copy_test);
}

TEST(BSort, easy_decr)
{
    std::vector<int> test = {1, 3, 2, 6, 5};
    std::vector<int> copy_test = test;

    clM::BSort(test, clM::Sort::Decr);
    std::sort(copy_test.begin(), copy_test.end(),
              [](const int &lhs, const int &rhs) { return lhs > rhs; });

    ASSERT_EQ(test, copy_test);
}

TEST(BSort, big_incr)
{
    constexpr size_t size = 100;
    constexpr size_t tests = 100;

    std::vector<int> test;
    test.resize(size);
    MLib::Random random{-1000, 1000};

    for (size_t i = 0; i < tests; ++i)
    {
        for (size_t j = 0; j < size; ++j)
            test[j] = random.get();

        std::vector<int> copy_test = test;
        clM::BSort(test, clM::Sort::Incr);
        std::sort(copy_test.begin(), copy_test.end());

        ASSERT_EQ(test, copy_test);
    }
}

TEST(BSort, big_decr)
{
    constexpr size_t size = 100;
    constexpr size_t tests = 100;

    std::vector<int> test;
    test.resize(size);
    MLib::Random random{-1000, 1000};

    for (size_t i = 0; i < tests; ++i)
    {
        for (size_t j = 0; j < size; ++j)
            test[j] = random.get();

        std::vector<int> copy_test = test;
        clM::BSort(test, clM::Sort::Decr);
        std::sort(copy_test.begin(), copy_test.end(),
                  [](const int &lhs, const int &rhs) { return lhs > rhs; });

        ASSERT_EQ(test, copy_test);
    }
}