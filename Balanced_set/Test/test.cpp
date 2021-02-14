#include <iostream>
#include <vector>
#include "../Libraries/base_data.hpp"
#include <gtest/gtest.h>
#include "../Common_libs/random.hpp"

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(DIFFERENCE, Check)
{
    base_data<int> arr;
    for (int i = 0; i < 20; ++i)
        arr.insert(i);

    EXPECT_EQ(arr.upper_bound(5) - arr.lower_bound(1), 5);
}

TEST(E2E, small_easy)
{
    base_data<int> arr;
    for (int i = 0; i < 20; ++i)
        arr.insert(i);

    for (int i = 0; i < 20; ++i)
    {
        EXPECT_EQ(i, arr.GetCountLess(i));
    }
}

TEST(E2E, small_hard)
{
    std::vector<int> arr = {1, 3, 4, 6, 10};
    base_data<int> set;
    for (int elem : arr)
        set.insert(elem);

    EXPECT_EQ(set.GetCountLess(0), 0);
    EXPECT_EQ(set.GetCountLess(1), 0);
    EXPECT_EQ(set.GetCountLess(2), 1);
    EXPECT_EQ(set.GetCountLess(3), 1);
    EXPECT_EQ(set.GetCountLess(6), 3);
    EXPECT_EQ(set.GetCountLess(10), 4);
    EXPECT_EQ(set.GetCountLess(11), 5);
    EXPECT_EQ(set.GetCountLess(1000), 5);
}

TEST(E2E, big_easy)
{
    base_data<int> arr;
    for (int i = 0; i < 20000; ++i)
        arr.insert(i);

    for (int i = 0; i < 20000; ++i)
    {
        EXPECT_EQ(i, arr.GetCountLess(i));
    }
}

TEST(E2E, big_hard)
{
    Random gen{-10000, 10000};

    std::set<int> set;
    for (int i = 0; i < 1000; ++i)
        set.insert(gen.get());

    base_data<int> my_set;
    for (int elem : set)
        my_set.insert(elem);

    Random gen2{-5000, 5000};
    for (int i = 0; i < 100; ++i)
    {
        int cur_test = gen2.get();
        EXPECT_EQ(std::distance(set.begin(), set.lower_bound(cur_test)), my_set.GetCountLess(cur_test));
    }
}

TEST(E2E, small_easy2)
{
    std::vector<int> arr = {5, 2, 7, 1, 9, 8, 6, 3, 4};
    base_data<int> set;
    for (int elem : arr)
        set.insert(elem);

    //set.InorderDump(std::cout);
    EXPECT_EQ(set.FindMin(0), set.end());
    EXPECT_EQ(set.FindMin(-6543), set.end());
    EXPECT_EQ(set.FindMin(653), set.end());

    for (int i = 1; i < 10; ++i)
        EXPECT_EQ(*set.FindMin(i), i);
}

TEST(E2E, big_easy2)
{
    Random rand_gen{0, 100000};

    std::set<int> arr;
    for (int i = 0; i < 1000; ++i)
        arr.insert(rand_gen.get());

    base_data<int> set;
    for (int elem : arr)
        set.insert(elem);

    int cur_num = 1;
    for (auto iter = arr.cbegin(); iter != arr.cend(); cur_num++, iter++)
    {
        EXPECT_EQ(*iter, *(set.FindMin(cur_num)));
    }
}

TEST(E2E, big_ord2)
{
    Random rand_gen{0, 100000};

    std::vector<int> data;
    std::set<int> arr;
    for (int i = 0; i < 1000; ++i)
        data.push_back(rand_gen.get());

    base_data<int> set;
    for (int elem : data)
    {
        set.insert(elem);
        arr.insert(elem);
    }

    int cur_num = 1;
    for (auto iter = arr.cbegin(); iter != arr.cend(); cur_num++, iter++)
    {
        EXPECT_EQ(*iter, *(set.FindMin(cur_num)));
    }
}