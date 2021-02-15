#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>
#include "../Common_libs/Random/Random.hpp"

int main()
{
    constexpr size_t size = 9;
    constexpr size_t count = 1000000;

    std::array<std::ofstream, size> files = {
        std::ofstream{"1.txt", std::ios_base::out},
        std::ofstream{"2.txt", std::ios_base::out},
        std::ofstream{"3.txt", std::ios_base::out},
        std::ofstream{"4.txt", std::ios_base::out},
        std::ofstream{"5.txt", std::ios_base::out},
        std::ofstream{"6.txt", std::ios_base::out},
        std::ofstream{"7.txt", std::ios_base::out},
        std::ofstream{"8.txt", std::ios_base::out},
        std::ofstream{"9.txt", std::ios_base::out},
    };

    MLib::Random rand(-1000, 1000);
    std::array<int, count> check;

    for (size_t i = 0; i < size; ++i)
    {
        files[i] << count << std::endl;

        for (int& elem : check)
        {
            elem = rand.get();
            files[i] << elem << " ";
        }
        
        files[i].close();
    }
}