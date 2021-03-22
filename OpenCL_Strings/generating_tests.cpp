#include <iostream>
#include <fstream>
#include "Common_libs/Random.hpp"
#include "Common_libs/Errors.hpp"
#include <array>

int main ()
{
    constexpr size_t size_files = 6;
    constexpr size_t count = (1 << 15);
    constexpr size_t tests = (1 << 15) ;
    MLib::Random rand_small (57 , 60); //ranges of random
    MLib::Random rand_middle (45 , 60);
    MLib::Random rand_big (40 , 100);
    size_t size_test = 16;

    //openening files
    std::array<std::ofstream , size_files> files;
    size_t cur_file = 0;
    for (std::ofstream& file : files)
    {
        file = std::ofstream { std::string ("../TESTS/") + std::to_string (cur_file) + ".txt" };
        if (!file.is_open ())
            ERROR ("File wasn't opened!");
        ++cur_file;
    }

    //!Small tests
    for (size_t i = 0; i < size_files; ++i)
    {
        //size
        files[i] << count;
        files[i] << " ";

        //base
        for (size_t j = 0; j < count; ++j)
            //rand > 0 and rand < 256 -> cast is good
            files[i] << static_cast<char>(rand_small.get ());

        //amount of tests in one file
        files[i] << "\n" << tests << "\n";

        //patterns
        for (size_t k = 0; k < tests; ++k)
        {
            files[i] << size_test << " ";
            for (size_t j = 0; j < size_test; ++j)
                files[i] << static_cast<char>(rand_small.get ());
            
            files[i] << "\n";
        }

        size_test <<= 1;
        files[i].close();
    }
}