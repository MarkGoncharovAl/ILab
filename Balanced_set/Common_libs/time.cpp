#include "time.hpp"

Time::Time()
    : start(std::chrono::high_resolution_clock::now())
{
}

std::chrono::microseconds
Time::GetAndResetTime()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    start = end;
    return dur;
}

std::chrono::microseconds
Time::Get()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
}