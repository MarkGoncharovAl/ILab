#include <iostream>
#include <fstream>
#include "Common_libs/Random.hpp"
#include "Common_libs/Errors.hpp"
#include <array>

int main ()
{
    constexpr size_t size_files = 6;
    constexpr size_t count = (1 << 16);
    constexpr size_t tests[3] = { 128, (1 << 10), (1 << 15) };
    MLib::Random rand_small (56 , 60); //ranges of random
    MLib::Random rand_middle (45 , 60);
    MLib::Random rand_big (40 , 100);
    size_t size_test = 1;

    //openening files
    std::array<std::ofstream , 9 * size_files> files;
    size_t cur_file = 0;
    for (std::ofstream& file : files)
    {
        file = std::ofstream { std::string ("../TESTS/") + std::to_string (cur_file) + ".txt" };
        if (!file.is_open ())
            ERROR ("File wasn't opened!");
        ++cur_file;
    }

    for (size_t l = 0; l < 3; ++l)
    {
        //!Small tests
        for (size_t i = 0; i < size_files; ++i)
        {
            //size
            files[18 * l + i] << count;
            files[18 * l + i] << " ";

            //base
            for (size_t j = 0; j < count; ++j)
            {
                //rand > 0 and rand < 256 -> cast is good
                files[18 * l + i] << static_cast<char>(rand_small.get ());
            }

            //amount of tests in one file
            files[18 * l + i] << "\n" << tests[l] << "\n";

            //patterns
            for (size_t k = 0; k < tests[l]; ++k)
            {
                files[18 * l + i] << size_test << " ";
                for (size_t j = 0; j < size_test; ++j)
                {
                    files[18 * l + i] << static_cast<char>(rand_small.get ());
                }
                files[18 * l + i] << "\n";
            }

            size_test <<= 1;
        }

        //!Middle tests
        size_test = 1;
        for (size_t i = size_files; i < size_files * 2; ++i)
        {
            //size
            files[18 * l + i] << count;
            files[18 * l + i] << " ";

            //base
            for (size_t k = 0; k < count; ++k)
            {
                //rand > 0 and rand < 256 -> cast is good
                files[18 * l + i] << static_cast<char>(rand_middle.get ());
            }

            //amount of tests in one file
            files[18 * l + i] << "\n" << tests[l] << "\n";

            //patterns
            for (size_t k = 0; k < tests[l]; ++k)
            {
                files[18 * l + i] << size_test << " ";
                for (size_t j = 0; j < size_test; ++j)
                {
                    files[18 * l + i] << static_cast<char>(rand_middle.get ());
                }
                files[18 * l + i] << "\n";
            }

            size_test <<= 1;
        }

        //!Big tests
        size_test = 1;
        for (size_t i = size_files * 2; i < size_files * 3; ++i)
        {
            //size
            files[18 * l + i] << count;
            files[18 * l + i] << " ";

            //base
            for (size_t k = 0; k < count; ++k)
            {
                //rand > 0 and rand < 256 -> cast is good
                files[18 * l + i] << static_cast<char>(rand_big.get ());
            }

            //amount of tests in one file
            files[18 * l + i] << "\n" << tests[l] << "\n";

            //patterns
            for (size_t k = 0; k < tests[l]; ++k)
            {
                files[18 * l + i] << size_test << " ";
                for (size_t j = 0; j < size_test; ++j)
                {
                    files[18 * l + i] << static_cast<char>(rand_big.get ());
                }
                files[18 * l + i] << "\n";
            }

            size_test <<= 1;
        }
    }
}