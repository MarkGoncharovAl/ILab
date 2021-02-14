#include "tests.hpp"

int RunTests()
{
    return RUN_ALL_TESTS();
}

TEST(INSERT, BALANCE)
{
    Binary_Heap<int> tree;
    EXPECT_TRUE(tree);
    tree.insert(0);
    ASSERT_FALSE(tree);
}

TEST(INSERT, FULL_DATA)
{
    Binary_Heap<int> tree;
    std::vector<int> arr = {1, -654, 2, 65, 32, 52, -42};
    for (const int elem : arr)
        EXPECT_TRUE(tree.insert(elem));

    auto data_tree = tree.GetFullData();
    ASSERT_EQ(arr, data_tree);
}

TEST(INSERT, SAME_DATA)
{
    Binary_Heap<int> tree;
    std::vector<int> arr = {1, 3};
    for (const int elem : arr)
        EXPECT_TRUE(tree.insert(elem));

    EXPECT_FALSE(tree.insert(1));

    auto data_tree = tree.GetFullData();
    ASSERT_EQ(arr, data_tree);
}