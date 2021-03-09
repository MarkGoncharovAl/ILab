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

TEST(DFS, easy)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}}};
    std::cout << graph;
    bool check_out = graph.IsBipartite();
    auto&& out = graph.GetColors();
    std::vector<int> check = {0, 1, 2};
    std::vector<char> check_color = {0, 1, 1};

    ASSERT_EQ(check_out, true);
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
    bool out = graph.IsBipartite();
    std::vector<int> check = {0, 1, 2};

    ASSERT_EQ(out, false);
}

TEST(DFS, is_2_easy_true_2)
{
    MC::KGraph<> graph{{{-1, 1}}};
    bool check_b = graph.IsBipartite();
    ASSERT_EQ(check_b, true);

    auto out = graph.GetColors();
    std::vector<MCGraph::Color::type> check = {0, 1};
    for (size_t i = 0; i < check.size(); ++i)
        ASSERT_EQ(out[i].second.data_, check[i]);
}

TEST(DFS, is_2_easy_false)
{
    MC::KGraph<> graph{{{0, 1}, {0, 2}, {1, 2}}};
    bool check_b = graph.IsBipartite();
    ASSERT_EQ(check_b, false);
}

TEST(DFS, is_2_ord)
{
    MC::KGraph<> graph{{{0, 1}, {2, 3}, {2, 1}}};
    bool check_b = graph.IsBipartite();
    ASSERT_EQ(check_b, true);
}

TEST(DFS, is_2_colors)
{
    using pair = std::pair<int, MCGraph::Color>;

    MC::KGraph<> graph{{{0, 1}, {2, 3}, {2, 1}}};
    bool check_b = graph.IsBipartite();
    ASSERT_EQ(check_b, true);

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

    bool check_b = graph.IsBipartite();
    ASSERT_EQ(check_b, true);

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
    bool bip = graph.IsBipartite();
    ASSERT_EQ(bip, false);

    auto&& checking = graph.ProveNotBipartite();
    std::vector<int> provement = {0, 1, 2};
    ASSERT_EQ(checking, provement);
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

    bool bip = graph.IsBipartite();
    ASSERT_EQ(bip, false);

    auto&& checking = graph.ProveNotBipartite();
    std::vector<int> out_check = {2, 3, 4};
    ASSERT_EQ(checking, out_check);
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

    bool bip = graph.IsBipartite();
    ASSERT_EQ(bip, false);

    auto&& checking = graph.ProveNotBipartite();
    std::vector<int> out_check = {0, 1, 2, 3, 4};
    ASSERT_EQ(checking, out_check);
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

    bool bip = graph.IsBipartite();
    ASSERT_EQ(bip, false);

    auto&& checking = graph.ProveNotBipartite();
    std::vector<int> out_check = {2, 3, 4, 5, 6};
    ASSERT_EQ(checking, out_check);
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

    bool bip = graph.IsBipartite();
    ASSERT_EQ(bip, true);
}

TEST(same_node, simple)
{
    MC::KGraph<> graph;
    graph.AddEdge(1, 1, 10);

    graph.dump(std::cout);
}

TEST(CopyCtor, simple)
{
    MC::KGraph<> graph({{0, 1}, {0, 2},
                        {2, 5}, {2, 3},
                        {3, 4}, {4, 5}});
    graph.dump(std::cout);
    MC::KGraph<> graph2{graph};
    graph2.dump(std::cout);
}