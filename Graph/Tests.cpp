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
    auto&& check_out = graph.DFS_Bip();
    auto&& out = graph.GetColors();
    std::vector<int> check = {0, 1, 2};
    std::vector<char> check_color = {0, 1, 1};

    ASSERT_EQ(check_out.is_biparatite_, true);
    for (size_t i = 0; i < check.size(); ++i)
    {
        //std::cout << out[i].first << std::endl;
        ASSERT_EQ(out[i].first, check[i]);
        ASSERT_EQ(out[i].second.data_, check_color[i]);
    }
}
TEST(DFS, easy2)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}, {1, 2}}};
    std::cout << graph;
    auto&& out = graph.DFS_Bip();
    std::vector<int> check = {0, 1, 2};

    ASSERT_EQ(out.is_biparatite_, false);
}

TEST(DFS, is_2_easy_true_2)
{
    MC::KGraph<> graph{{{-1, 1}}};
    auto&& check_b = graph.DFS_Bip();
    ASSERT_EQ(check_b.is_biparatite_, true);

    auto out = graph.GetColors();
    std::vector<MCGraph::Color::type> check = {0, 1};
    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].second.data_, check[i]);
}

TEST(DFS, is_2_easy_false)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}, {1, 2}}};
    auto&& check_b = graph.DFS_Bip();
    ASSERT_EQ(check_b.is_biparatite_, false);
}

TEST(DFS, is_2_ord)
{
    MC::KGraph<> graph{{{0, 1}, {2, 3}, {2, 1}}};
    auto&& check_b = graph.DFS_Bip();
    ASSERT_EQ(check_b.is_biparatite_, true);
}

TEST(DFS, is_2_colors)
{
    using pair = std::pair<int, MCGraph::Color>;

    MC::KGraph<> graph{{{0, 1}, {2, 3}, {2, 1}}};
    auto check_b = graph.DFS_Bip();
    ASSERT_EQ(check_b.is_biparatite_, true);

    auto out = graph.GetColors();
    std::vector<MCGraph::Color::type> check =
        {0, 1, 0, 1};

    std::sort(out.begin(), out.end(),
              [](const pair &lhs, const pair &rhs) { return lhs.first < rhs.first; });

    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].second.data_, check[i]);
}

TEST(DFS, is_2_colors_HARD)
{
    using pair = std::pair<int, MCGraph::Color>;

    // 1    6
    // 2    7
    // 3    8
    // 4    9
    // 5    10
    MC::KGraph<> graph{{{1, 6}, {1, 7}, {2, 9}, {2, 10}, {3, 7}, {3, 8}, {4, 7}, {4, 9}, {5, 6}, {5, 10},
                        //extra
                        {1, 10},
                        {3, 6}}};

    auto check_b = graph.DFS_Bip();
    ASSERT_EQ(check_b.is_biparatite_, true);

    auto out = graph.GetColors();
    std::vector<MCGraph::Color::type> check{10};
    for (size_t i = 0; i < 5; ++i)
        check[i] = MCGraph::Color::BLUE;
    for (size_t i = 5; i < 10; ++i)
        check[i] = MCGraph::Color::RED;

    std::sort(out.begin(), out.end(),
              [](const pair &lhs, const pair &rhs) { return lhs.first < rhs.first; });

    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].second.data_, check[i]);
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
    graph.AddEdge(4, 5);
    //std::cout << graph;
    graph.AddEdge(1, 5);
    //std::cout << graph;
    graph.AddEdge(0, 4);
    //std::cout << graph;
}

TEST(DFS, Hard1)
{
    MC::KGraph<> graph({{0, 1}, {0, 2}, {1, 2}});
    auto&& bip = graph.DFS_Bip();
    ASSERT_EQ(bip.is_biparatite_, false);
    std::vector<int> provement = {0, 1, 2};
    ASSERT_EQ(bip.prove_, provement);
}

TEST(DFS, Hard2)
{
    /*
    0
    |----------|
    1          2
    |     |----|
    3------    |
    |          |
    -----------4
    */
    MC::KGraph<> graph({{0, 1}, {0, 2},
                        {1, 3}, 
                        {2, 3}, {2, 4},
                        {3, 4}});

    auto&& bip = graph.DFS_Bip();
    ASSERT_EQ(bip.is_biparatite_, false);
    std::vector<int> out_check = {2, 3, 4};
    ASSERT_EQ(bip.prove_, out_check);
}

TEST(DFS, Hard3)
{
    /*
    0----4-----3
    |          |
    1----------2
    */
    MC::KGraph<> graph({{0, 1}, {1, 2},
                        {2, 3}, {3, 4}, {4, 0}});

    auto&& bip = graph.DFS_Bip();
    ASSERT_EQ(bip.is_biparatite_, false);
    std::vector<int> out_check = {0, 1, 2, 3, 4};
    // for (const int elem : bip.prove_)
    //     std::cout << elem << " ";
    // std::cout << "\n";
    ASSERT_EQ(bip.prove_, out_check);
}

TEST(DFS, Hard4)
{
    /*
    0----1
    |    |
    2----6-----5
    |          |
    3----------4
    */
    MC::KGraph<> graph({{0, 1}, {0, 2},
                        {2, 3}, {3, 4},
                        {4, 5}, {5, 6}, {6, 2}, {1, 6}});

    auto&& bip = graph.DFS_Bip();
    ASSERT_EQ(bip.is_biparatite_, false);
    std::vector<int> out_check = {2, 3, 4, 5, 6};
    // for (const int elem : bip.prove_)
    //     std::cout << elem << " ";
    // std::cout << "\n";
    ASSERT_EQ(bip.prove_, out_check);
}

TEST(DFS, Hard5)
{
    /*
    0----1
    |    |
    2----5
    |    |
    3----4
    */
    MC::KGraph<> graph({{0, 1}, {0, 2},
                        {2, 5}, {2, 3},
                        {3, 4}, {4, 5}});

    auto&& bip = graph.DFS_Bip();
    ASSERT_EQ(bip.is_biparatite_, true);
}