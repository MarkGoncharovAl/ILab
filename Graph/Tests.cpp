#include <iostream>
#include <algorithm>
#include <gtest/gtest.h>
#include "KGraph/KGraph.hpp"
#include "Common_libs/Random/Random.hpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Dump, lesson)
{
    //был на уроке
    MC::KGraph<> graph{{{0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}}};
    std::cout << graph;
}

TEST(WFS, easy)
{
    MC::KGraph<> graph{{{1, 2}, {1, 3}}};
    std::cout << graph;

    auto out = graph.dumpWFS();

    std::vector<std::pair<int, int>> check = {{1, 0}, {2, 1}, {3, 1}};
    ASSERT_EQ(out, check);
}

TEST(WFS, ordinary)
{
    //        0
    //  1 ----|---- 2
    //  |           |
    //  |---|   |---|
    //  |   |   |   |
    //  3   4---5   6
    //  |           |
    //  7           8
    MC::KGraph<> graph{{{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}, {3, 7}, {4, 5}, {6, 8}}};
    std::cout << graph;

    auto out = graph.dumpWFS();

    std::vector<std::pair<int, int>> check = {
        {0, 0},
        {1, 1},
        {2, 1},
        {3, 2},
        {4, 2},
        {5, 2},
        {6, 2},
        {7, 3},
        {8, 3}};
    ASSERT_EQ(out, check);
}

TEST(DFS, easy)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}}};
    std::cout << graph;
    auto out = graph.dumpDFS();
    std::vector<int> check = {0, 1, 2};

    for (size_t i = 0; i < check.size(); ++i)
    {
        //std::cout << out[i].first << std::endl;
        ASSERT_EQ(out[i].first, check[i]);
    }
}

TEST(DFS, ordinary)
{
    //        0
    //  1 ----|---- 2
    //  |           |
    //  |---|   |---|
    //  |   |   |   |
    //  3   4---5   6
    //  |           |
    //  7           8
    MC::KGraph<> graph{{{0, 1}, {0, 2}, {1, 3}, {1, 4}, {2, 5}, {2, 6}, {3, 7}, {4, 5}, {6, 8}}};
    auto out = graph.dumpDFS();

    std::vector<int> check = {0, 1, 3, 7, 4, 5, 2, 6, 8};
    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].first, check[i]);
}

TEST(DFS, is_2_easy_true)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}}};
    bool check_b = false;
    auto out = graph.dumpDFS(&check_b);
    ASSERT_EQ(check_b, true);

    std::vector<int> check = {0, 1, 2};
    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].first, check[i]);
}

TEST(DFS, is_2_easy_true_2)
{
    MC::KGraph<> graph{{{-1, 1}}};
    bool check_b = false;
    auto out = graph.dumpDFS(&check_b);
    ASSERT_EQ(check_b, true);

    std::vector<int> check = {-1, 1};
    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].first, check[i]);
}

TEST(DFS, is_2_easy_false)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}, {1, 2}}};
    bool check_b = true;
    auto out = graph.dumpDFS(&check_b);
    ASSERT_EQ(check_b, false);
}

TEST(DFS, is_2_ord)
{
    MC::KGraph<> graph{{{0, 1}, {2, 3}, {2, 1}}};
    bool check_b = false;
    auto out = graph.dumpDFS(&check_b);
    ASSERT_EQ(check_b, true);
}

TEST(DFS, is_2_colors)
{
    using pair = std::pair<int, typename MC::KGraph<>::Color>;

    MC::KGraph<> graph{{{0, 1}, {2, 3}, {2, 1}}};
    auto out = graph.dumpDFS();

    std::vector<typename MC::KGraph<>::Color> check =
        {MC::KGraph<>::Color::Blue, MC::KGraph<>::Color::Red, MC::KGraph<>::Color::Blue, MC::KGraph<>::Color::Red};

    std::sort(out.begin(), out.end(),
              [](const pair &lhs, const pair &rhs) { return lhs.first < rhs.first; });

    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].second, check[i]);
}

TEST(DFS, is_2_colors_HARD)
{
    using pair = std::pair<int, typename MC::KGraph<>::Color>;

    // 1    6
    // 2    7
    // 3    8
    // 4    9
    // 5    10
    MC::KGraph<> graph{{{1, 6}, {1, 7}, {2, 9}, {2, 10}, {3, 7}, {3, 8}, {4, 7}, {4, 9}, {5, 6}, {5, 10},
                        //extra
                        {1, 10},
                        {3, 6}}};

    bool out_check = false;
    auto out = graph.dumpDFS(&out_check);
    ASSERT_EQ(out_check, true);

    std::vector<typename MC::KGraph<>::Color> check{10};
    for (size_t i = 0; i < 5; ++i)
        check[i] = MC::KGraph<>::Color::Blue;
    for (size_t i = 5; i < 10; ++i)
        check[i] = MC::KGraph<>::Color::Red;

    std::sort(out.begin(), out.end(),
              [](const pair &lhs, const pair &rhs) { return lhs.first < rhs.first; });

    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].second, check[i]);
}

TEST(Add_Node, easy)
{
    MC::KGraph<> graph;
    graph.AddEdge(0, 1);
    //std::cout << graph;
    graph.AddEdge(2, 3);
    //std::cout << graph;
}

TEST(Add_Node, checking)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}}};
    //std::cout << graph;
    graph.AddEdge(0, 3);
    //std::cout << graph;
    //graph.AddEdge(4, 5);
    std::cout << graph;
   // graph.AddEdge(1, 5);
    std::cout << graph;
    //graph.AddEdge(0, 4);
    std::cout << graph;
}