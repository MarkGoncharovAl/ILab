#include <iostream>
#include <fstream>
#include "Common_libs/Random.hpp"
#include "Common_libs/Errors.hpp"
#include <array>

int main ()
{
    constexpr size_t size_files = 5;
    constexpr size_t count = (1 << 25);
    constexpr size_t tests = (1 << 2);

    //openening files
    std::array<std::ofstream , size_files> files;
    size_t cur_file = 0;
    for (std::ofstream& file : files)
    {
        file = std::ofstream { std::string("../TESTS/") + std::to_string (cur_file) + ".txt" };
        if (!file.is_open ())
            ERROR ("File wasn't opened!");
        ++cur_file;
    }

    MLib::Random rand (33 , 126);
    size_t size_test = 8;

    for (std::ofstream& file : files)
    {
        //size
        file << count;
        file << " ";

        //base
        for (size_t i = 0; i < count; ++i)
        {
            //rand > 0 and rand < 256 -> cast is good
            file << static_cast<char>(rand.get ());
        }

        //amount of tests in one file
        file << "\n" << tests << "\n";

        //patterns
        for (size_t i = 0; i < tests; ++i)
        {
            file << size_test << " ";
            for (size_t j = 0; j < size_test; ++j)
            {
                file << static_cast<char>(rand.get ());
            }
            file << "\n";
        }

        size_test <<= 1;

    }
}