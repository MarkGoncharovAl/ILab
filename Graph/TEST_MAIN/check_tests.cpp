#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <vector>
#include "../KGraph/KGraph.hpp"

std::vector<std::pair<int, int>> ReadData(std::ifstream &file);
std::vector<std::pair<int, MCGraph::Color>> ReadData_col(std::ifstream &file);

int main()
{
    constexpr size_t count = 10;
    for (size_t i = 0; i < count; ++i)
    {
        std::ifstream file{"../TEST_MAIN/" + std::to_string(i) + ".txt"};
        std::ifstream file_check{"../TEST_MAIN/" + std::to_string(i) + "_check.txt"};

        if (!file.is_open() || !file_check.is_open())
        {
            std::cout << "Problem with file!\n";
            return 1;
        }

        auto data = ReadData(file);
        MC::KGraph<> graph;
        for (auto elem : data)
            graph.AddEdge(elem.first, elem.second);
        
        if (graph.DFS_Bip().is_biparatite_ == false)
        {
            WARNING("Not biparatite!");
            return 0;
        }

        auto data_DFS = graph.GetColors();
        std::sort(data_DFS.begin(), data_DFS.end(),
                  [](const std::pair<int, MCGraph::Color> &lhs, const std::pair<int, MCGraph::Color> &rhs) { return lhs.first < rhs.first; });

        auto data_check = ReadData_col(file_check);

        // for (auto elem : data_DFS)
        // {
        //     std::cout << elem.first;
        //     switch (elem.second)
        //     {
        //     case MC::KGraph<>::Color::Blue:
        //         std::cout << "b\n";
        //         break;
        //     case MC::KGraph<>::Color::Red:
        //         std::cout << "b\n";
        //         break;
        //     default:
        //         break;
        //     }
        // }

        if (data_check == data_DFS)
            std::cout << MLib::Color::Under_Green << "Data is equal!\n"
                      << MLib::Color::Reset;
        else
            WARNING("Not equal data!");

        file.close();
        file_check.close();
    }
}

std::vector<std::pair<int, int>> ReadData(std::ifstream &file)
{
    std::vector<std::pair<int, int>> out;

    while (!file.eof())
    {
        std::pair<int, int> pair;
        int trash = 0;

        file >> pair.first;
        file.ignore(3);
        file >> pair.second;
        file.ignore(1);
        file >> trash;

        if (file.good())
            out.push_back(std::move(pair));
    }

    return out;
}

std::vector<std::pair<int, MCGraph::Color>> ReadData_col(std::ifstream &file)
{
    std::vector<std::pair<int, MCGraph::Color>> out;
    while (file && !file.eof() && file.good())
    {
        std::pair<int, MCGraph::Color> pair;
        char elem = '0';

        file >> pair.first;
        file >> elem;

        switch (elem)
        {
        case 'b':
            pair.second.data_ = MCGraph::Color::BLUE;
            break;
        case 'r':
            pair.second.data_ = MCGraph::Color::RED;
            break;
        default:
            return out;
        }

        if (file.good())
            out.push_back(std::move(pair));
    }

    return out;
}