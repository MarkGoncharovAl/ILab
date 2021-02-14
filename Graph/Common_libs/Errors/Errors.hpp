#pragma once
#include <iostream>
#include "../Color.hpp"

namespace MLib
{
    [[noreturn]] inline void print_error(std::string info, std::string FILE, std::size_t LINE)
    {
        std::cout << std::endl
                  << info << std::endl;
        std::cout << MLib::Color::Under_Red << "Mistake was found in file " << FILE;
        std::cout << "\nMistake was found in line " << LINE << std::endl;
        exit(EXIT_FAILURE);
    }
    inline void print_warning(std::string info, std::string FILE, std::size_t LINE)
    {
        std::cout << std::endl
                  << info << std::endl;
        std::cout << MLib::Color::Under_Red << "Warning was found in file " << FILE;
        std::cout << "\nWarning was found in line " << LINE << std::endl << MLib::Color::Reset;
    }

    template <class T>
    inline void debug_print(const T &elem1, const T &elem2 = T{})
    {
        std::cout << MLib::Color::Purple << "Debagging is starting!\n"
                  << elem1 << "\t" << elem2 << "\nDebagging is ending" << MLib::Color::Reset; 
    }
} // namespace MLib

#define ERROR(str) MLib::print_error(str, __FILE__, __LINE__)
#define WARNING(str) MLib::print_warning(str, __FILE__, __LINE__)

#define DEBUG1(elem1) MLib::debug_print(elem1)
#define DEBUG2(elem1, elem2) MLib::debug_print(elem1, elem2)
