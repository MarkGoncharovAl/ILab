#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Common_libs/Time.hpp"

#include "NativeAlg/NativeAlg.hpp"
#include "NativeAlg_GPU/NativeAlg_GPU.hpp"
#include "RabKar/RabKar.hpp"

static std::string ReadBase ();
static clM::RabKar_Strings ReadPatterns ();

int main (int argc , char* argv [])
{
    cl::Device device = clM::ChooseDevice (argc , argv);
    if (device == cl::Device {})
    {
        LOG_warning << "Ending programm - device wasn't chosen";
        return 0;
    }
    LOG_trace << "device has been chosen";

    std::string base = ReadBase ();
    auto&& patterns = ReadPatterns ();
    auto&& patterns_vector = patterns.VecStrings ();

    try
    {
        clM::RabKar rabkar (device);

        MLib::Time time;

        LOG_trace << "Starting algorithm";
        time.Reset ();
        auto&& native = MLib::FindStrings (base , patterns_vector);
        //auto&& cur_time = time.GetAndResetTime ().count ();
        //std::cout << "Native: " << cur_time << std::endl;

        time.Reset ();
        auto&& check = rabkar.FindPatterns (base , patterns_vector);
        //cur_time = time.GetAndResetTime ().count ();
        //std::cout << "Rabkar: " << cur_time << std::endl;
        //std::cout << "Time in GPU: " << rabkar.GetKernelTime() << std::endl;

        LOG_trace << "Starting checking asnwers";
        for (size_t i = 0; i < native.size (); ++i)
        {
            //std::cout << native[i] << std::endl;
            if (check[i] != native[i])
            {
                std::cout << "NOT Equal: TEST: " << std::to_string (i)
                    << " : Native: ";
                std::cout << native[i] << ", Rabkar: " << check[i] << std::endl;
            }
            else //equal
            {
                std::cout << i + 1 << " " << check[i] << "\n";
            }
        }
    }
    catch (cl::Error& err)
    {
        LOG_fatal << err.what();
        clM::CheckReturnError (err.err ());
    }
    catch (std::exception& err)
    {
        LOG_fatal << err.what ();
    }

    return 0;
}

std::string ReadBase ()
{
    size_t count = 0;
    std::string out;

    std::cin >> count;
    if (!std::cin.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

    out.reserve (count);
    for (size_t i = 0; i < count; ++i)
    {
        char elem = 0;
        std::cin >> elem;
        out.push_back (elem);
    }
    if (!std::cin.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

    LOG_debug << "Reading base was correct";
    return out;
}

clM::RabKar_Strings ReadPatterns ()
{
    clM::RabKar_Strings output;
    size_t count = 0;

    std::cin >> count;
    if (!std::cin.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));
    output.nums_.reserve (count);

    for (size_t i = 0; i < count; ++i)
    {
        size_t size = 0;
        std::string out;

        std::cin >> size;
        if (!std::cin.good ())
            throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

        out.reserve (size);
        for (size_t i = 0; i < size; ++i)
        {
            char elem = 0;
            std::cin >> elem;
            out.push_back (elem);
        }
        if (!std::cin.good ())
            throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

        output.AddString (std::move (out));
    }

    LOG_debug << "Reading patterns was correct";
    return output;
}
