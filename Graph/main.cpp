#include <iostream>
#include "KGraph/KGraph.hpp"
#include <algorithm>
#include <tuple>

std::vector<std::tuple<int, int, int>> ReadData();

int main()
{
    auto data = ReadData();
    MC::KGraph<> graph;
    for (auto elem : data)
        graph.AddEdge(std::get<0>(elem), std::get<1>(elem), std::get<2>(elem));

    //graph.dump(std::cout);
    bool is_2 = false;
    auto checking = graph.DFS_Bip(&is_2);

    if (!is_2)
    {
        std::cout << "Graph not dicotyledonous!\n";
        return 0;
    }

    //is_2 == true
    std::sort(checking.begin(), checking.end(),
              [](const std::pair<int, MC::KGraph<>::Color> &lhs, const std::pair<int, MC::KGraph<>::Color> &rhs) { return lhs.first < rhs.first; });

    for (auto pair : checking)
    {
        std::cout << pair.first;
        if (pair.second == MC::KGraph<>::Color::Blue)
            std::cout << " b ";
        else
            std::cout << " r ";
        std::cout << std::endl;
    }
}

std::vector<std::tuple<int, int, int>> ReadData()
{
    std::vector<std::tuple<int, int, int>> out;

    while (std::cin)
    {
        int elem1 = 0, elem2 = 0, elem3 = 0;

        std::cin >> elem1;
        std::cin.ignore(3);
        std::cin >> elem2;
        std::cin.ignore(1);
        std::cin >> elem3;

        if (std::cin.good())
            out.push_back(std::move(std::make_tuple(elem1, elem2, elem3)));
    }

    return out;
}