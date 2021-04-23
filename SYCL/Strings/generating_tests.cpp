#include <iostream>
#include <fstream>
#include "Common_libs/Random.hpp"
#include "Common_libs/Logging/Logging.hpp"
#include <array>

int main ()
{
    constexpr size_t size_files = 6;
    constexpr size_t count = (1 << 12);
    constexpr size_t tests = (1 << 15) ;
    MLib::Random rand_small (58 , 60); //ranges of random
    MLib::Random rand_size(30, 100);

    //openening files
    std::array<std::ofstream , size_files> files;
    size_t cur_file = 0;
    for (std::ofstream& file : files)
    {
        file = std::ofstream { std::string ("../TESTS/") + std::to_string (cur_file) + ".txt" };
        if (!file.is_open ())
        {
            LOG_fatal << "File " << cur_file << " wasn't opened!";
        }
        ++cur_file;
    }

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
            size_t size_test = rand_size.get ();
            files[i] << size_test << " ";
            for (size_t j = 0; j < size_test; ++j)
                files[i] << static_cast<char>(rand_small.get ());
            
            files[i] << "\n";
        }

        files[i].close();
    }
}