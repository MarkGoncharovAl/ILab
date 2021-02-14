#pragma once
#include <iostream>
#include <random>

class Random
{
public:
    Random(int init_start = std::mt19937::min(), int init_end = std::mt19937::max()) noexcept;

    int get() noexcept;

private:
    static std::random_device rd_;
    static std::mt19937 gen_;

    std::uniform_int_distribution<> dist_;
};