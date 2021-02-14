#pragma once

#include <iostream>
#include <chrono>

class Time
{
public:
    Time();
    std::chrono::microseconds GetAndResetTime();
    std::chrono::microseconds Get();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};