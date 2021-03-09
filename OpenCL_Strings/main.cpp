#include "MC_OpenCL/MC_OpenCL.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "Common_libs/Errors.hpp"
#include "NativeAlg/NativeAlg.hpp"
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
        for (size_t i = 0; i < patterns.size(); ++i)
            std::cout << i << " " << MLib::FindStrings(base, patterns[i]) << "\n";
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

        output.push_back(out);
    }

    return output;
}
