#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Common_libs/Errors.hpp"
#include "Common_libs/Time.hpp"

#include "NativeAlg/NativeAlg.hpp"
#include "RabKar/RabKar.hpp"
#include <openssl/sha.h>

static std::string ReadBase ();
static std::vector<std::string> ReadPatterns ();

int main (int argc , char* argv [])
{
    cl::Device device = clM::ChooseDevice (argc , argv);
    if (device == cl::Device {})
        return 0;

    std::string base = ReadBase ();
    auto&& patterns = ReadPatterns ();

    try
    {
        clM::RabKar rabkar (std::move (device));

        MLib::Time time;
        auto&& native = MLib::FindStrings (base , patterns);
        std::cout << "Native: " << time.GetAndResetTime ().count () << std::endl;
        auto&& check = rabkar.FindPatterns (base , patterns);
        std::cout << "Rabkar: " << time.GetAndResetTime ().count () << std::endl;

        for (size_t i = 0; i < native.size (); ++i)
        {
            if (check[i] == native[i])
                std::cout << "Equal!" << std::endl;
            else
                std::cout << "Not equal!" << std::endl;
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

std::vector<std::string> ReadPatterns ()
{
    size_t count = 0;
    std::string out;

    std::cin >> count;
    if (!std::cin.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

    std::vector<std::string> output;
    output.reserve (count);

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

        output.push_back (out);
    }

    return output;
}
