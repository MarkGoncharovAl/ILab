#pragma once
#include <iostream>
#include "../Color.hpp"

namespace MLib
{
    [[noreturn]] void print_error(std::string info, std::string FILE, std::size_t LINE);
    void print_warning(std::string info, std::string FILE, std::size_t LINE);

    template <class T>
    inline void debug_print(const T &elem1, const T &elem2 = T{})
    {
        std::cout << Color::Under_Purple << "Debagging is starting!\n" << Color::Reset << 
                  << elem1 << "\t" << elem2 
                  << Color::Under_Purple << "\nDebagging is ending" << Color::Reset;
    }
} // namespace help

#define ERROR(str) MLib::print_error(str, __FILE__, __LINE__)
#define WARNING(str) MLib::print_warning(str, __FILE__, __LINE__)

#define DEBUG1(elem1) MLib::debug_print(elem1)
#define DEBUG2(elem1, elem2) MLib::debug_print(elem1, elem2)
