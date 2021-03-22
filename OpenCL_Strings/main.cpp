#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Common_libs/Errors.hpp"
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
        return 0;

    std::string base = ReadBase ();
    auto&& patterns = ReadPatterns ();
    auto&& patterns_vector = patterns.VecStrings();

    try
    {
        clM::RabKar rabkar (device);

        MLib::Time time;
        
        time.Reset();
        auto&& native = MLib::FindStrings (base , patterns_vector);
        auto&& cur_time = time.GetAndResetTime().count();
        std::cout << "Native: " << cur_time << std::endl;
        
        time.Reset();
        auto&& check = rabkar.FindPatterns (base , patterns_vector);
        cur_time = time.GetAndResetTime().count();
        std::cout << "Rabkar: " << cur_time << std::endl;
        
        rabkar.HashEffect();

        for (size_t i = 0; i < native.size (); ++i)
        {
            //std::cout << native[i] << std::endl;
            if (check[i] != native[i])
            {
                std::cout << "Not Equal: " << std::to_string (i)
                << std::endl;
                std::cout << native[i] << " : " << check[i] << std::endl;
            }
        }
    }
    catch (cl::Error& err)
    {
        std::cerr << err.what () << std::endl;
        clM::CheckErr (err.err ());
    }
    catch (std::exception& err)
    {
        std::cerr << err.what () << std::endl;
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

        output.AddString(std::move(out));
    }

    return output;
}
