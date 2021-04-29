#pragma once
#include <iostream>
#include <random>

namespace MLib
{
    class Random
    {
    public:
        Random (int init_start = std::mt19937::min () , int init_end = std::mt19937::max ())
            : rd_ () ,
            gen_ (rd_()) ,
            dist_ (init_start , init_end)
        {}

        int get () { return dist_ (gen_); }

    private:
        std::random_device rd_;
        std::mt19937 gen_;
        std::uniform_int_distribution<> dist_;
    };

} // namespace MLib