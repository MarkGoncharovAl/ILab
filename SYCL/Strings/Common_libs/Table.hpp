#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <array>
#include <tuple>

namespace MLib
{
    template <typename T , size_t width>
    class Table_h
    {
    public:
        std::array<std::string , width> header_;
        std::vector<std::array<T , width>> data_;
        size_t spaces_ = 15;
        char filling_ = ' ';

        void Print ();
    };

    template <typename T , size_t width>
    void Table_h<T , width>::Print ()
    {
        // +2 for []
        for (size_t i = 0; i < (spaces_ + 2) * width; ++i)
            std::cout << "_";
        std::cout << std::endl;

        for (size_t i = 0; i < width; ++i)
            std::cout << "[" << std::setfill (filling_) << std::setw (spaces_) << header_[i] << std::right << "]";
        std::cout << std::endl;

        for (size_t i = 0; i < (spaces_ + 2) * width; ++i)
            std::cout << "_";
        std::cout << std::endl;

        for (auto&& raw : data_)
        {
            for (size_t i = 0; i < width; ++i)
                std::cout << "[" << std::setfill (filling_) << std::setw (spaces_) << raw[i] << std::right << "]";

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}