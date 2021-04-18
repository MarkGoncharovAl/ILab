#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Common_libs/Time.hpp"

#include "NativeAlg/NativeAlg.hpp"
#include "RabKar/RabKar.hpp"

static std::string ReadBase ();
static std::vector<std::string> ReadPatterns ();

int main (int argc , char* argv [])
{
    std::string base = ReadBase ();
    auto&& patterns = ReadPatterns ();

    try
    {
        Msycl::RabKar rabkar (base);

        MLib::Time time;

        LOG_trace << "Starting algorithm";
        time.Reset ();
        auto&& native = MLib::FindStrings (base , patterns);
        auto&& cur_time = time.GetAndResetTime ().count ();
        std::cout << "Native: " << cur_time << std::endl;

        time.Reset ();
        auto&& check = rabkar.FindPatterns (patterns);
        cur_time = time.GetAndResetTime ().count ();
        std::cout << "Rabkar: " << cur_time << std::endl;
        std::cout << "Time in GPU: " << rabkar.GetKernelTime() << std::endl;

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
    catch (cl::sycl::exception& err)
    {
        LOG_fatal << err.what();
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

std::vector<std::string> ReadPatterns ()
{
    std::vector<std::string> output;
    size_t count = 0;

    std::cin >> count;
    if (!std::cin.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));
    output.reserve (count);

    for (size_t i = 0; i < count; ++i)
    {
        size_t size = 0;
        std::string out;

        std::cin >> size;
        if (!std::cin.good ())
            throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

        out.reserve (size);
        std::cin >> out;
        // if (!std::cin.good ())
            // throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

        output.push_back (std::move (out));
    }

    LOG_debug << "Reading patterns was correct";
    return output;
}
