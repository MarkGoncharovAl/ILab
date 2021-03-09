#pragma once

#include <iostream>
#include <chrono>

namespace MLib
{
    class Time
    {
    public:
        Time();

        void Reset();
        std::chrono::microseconds GetAndResetTime();
        std::chrono::microseconds Get();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    };

} // namespace MLib