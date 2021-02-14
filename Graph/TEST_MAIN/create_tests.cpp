#include <iostream>
#include <fstream>
#include <set>
#include "../Common_libs/Random/Random.hpp"

int main()
{
    constexpr size_t count = 10;
    for (size_t i = 0; i < count; ++i)
    {
        std::ofstream file{std::to_string(i) + ".txt"};
        std::ofstream file_check{std::to_string(i) + "_check.txt"};

        if (!file.is_open() || !file_check.is_open())
        {
            std::cout << "Problem with file!\n";
            return 1;
        }

        MLib::Random rand1{1, 10000};
        MLib::Random rand2{-100, -1};
        MLib::Random rand3{1, 100};

        int size = rand1.get();

        std::set<int> set1;
        std::set<int> set2;

        for (int j = 0; j < size; ++j)
        {
            int num1 = rand2.get();
            int num2 = rand3.get();
            set1.insert(num1);
            set2.insert(num2);
            file << num1 << " -- " << num2 << ", " << 0 << std::endl;
        }

        for (int elem : set1)
            file_check << elem << " b ";
        for (int elem : set2)
            file_check << elem << " r ";

        file.close();
        file_check.close();
    }
}