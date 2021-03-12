#pragma once

#include <iostream>
#include <chrono>

namespace MLib
{
    class Time
    {
    public:
        Time ()
            : start (std::chrono::high_resolution_clock::now ())
        {}

        void Reset ();
        std::chrono::microseconds GetAndResetTime ();
        std::chrono::microseconds Get ();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    };

    //Realizations
    inline std::chrono::microseconds
        Time::GetAndResetTime ()
    {
        auto end = std::chrono::high_resolution_clock::now ();
        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        start = end;
        return dur;
    }

    inline std::chrono::microseconds
        Time::Get ()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now () - start);
    }

    inline void Time::Reset ()
    {
        start = std::chrono::high_resolution_clock::now ();
    }

} // namespace MLib