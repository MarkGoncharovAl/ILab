#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#include "Common_libs/Errors.hpp"
#include "Common_libs/Time.hpp"

#include "NativeAlg/NativeAlg.hpp"
#include "NativeAlg_GPU/NativeAlg_GPU.hpp"
#include "RabKar/RabKar.hpp"
#include <openssl/sha.h>

static std::string ReadBase (std::ifstream& file);
static clM::RabKar_Strings ReadPatterns (std::ifstream& file);

int main (int argc , char* argv [])
{
    cl::Device device = clM::ChooseDevice (argc , argv);
    if (device == cl::Device {})
        return 0;

    //preparing files
    std::array<std::ifstream , 36> files;
    size_t cur_file = 0;
    for (std::ifstream& file : files)
    {
        file = std::ifstream { std::string ("../TESTS/") + std::to_string (cur_file) + ".txt" };
        if (!file.is_open ())
            ERROR ("File wasn't opened!");
        ++cur_file;
    }

    std::cout << "|Native\t\t|RabKar\t\t|Native_GPU\t|\n";

    for (auto&& file : files)
    {
        std::string base = ReadBase (file);
        auto&& patterns = ReadPatterns (file);
        auto&& patterns_vector = patterns.VecStrings ();

        try
        {
            clM::RabKar rabkar (device);
            clM::NativeAlg_GPU native_GPU (device);

            MLib::Time time;

            time.Reset ();
            auto&& native = MLib::FindStrings (base , patterns_vector);
            double time_native = time.GetAndResetTime ().count ();
            std::cout << "|" << time_native << "\t\t|";

            time.Reset ();
            auto&& check = rabkar.FindPatterns (base , patterns_vector);
            double time_check = time.GetAndResetTime ().count ();
            std::cout << time_check << "\t\t|";

            time.Reset ();
            auto&& check_GPU = native_GPU.FindPatterns (base , patterns);
            double time_nativeGPU = time.GetAndResetTime ().count ();
            std::cout << time_nativeGPU << "\t\t|";

            bool everything_is_equal = true;
            for (size_t i = 0; i < native.size (); ++i)
            {
                //std::cout << native[i] << std::endl;
                if (check[i] != native[i] || check_GPU[i] != native[i])
                {
                    std::cout << "\nNot Equal: " << std::to_string (i)
                        << std::endl;
                    std::cout << native[i] << " : " << check[i] << " : " << check_GPU[i];
                    everything_is_equal = false;
                }
            }
            if (everything_is_equal)
            {
                if (time_native <= time_check)
                {
                    if (time_native <= time_nativeGPU)
                        std::cout << "native";
                    else
                        std::cout << "native_GPU in " << time_native / time_nativeGPU << " times";
                }
                else
                { //time_native > time_check
                    if (time_check <= time_nativeGPU)
                        std::cout << "rabkar in " << time_native / time_check << " times";
                    else
                        std::cout << "native_GPU in " << time_native / time_nativeGPU << " times";
                }
            }

            std::cout << std::endl;
            file.close ();
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

    }
    return 0;
}

std::string ReadBase (std::ifstream& file)
{
    size_t count = 0;
    std::string out;

    file >> count;
    if (!file.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

    out.reserve (count);
    for (size_t i = 0; i < count; ++i)
    {
        char elem = 0;
        file >> elem;
        out.push_back (elem);
    }
    if (!file.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

    return out;
}

clM::RabKar_Strings ReadPatterns (std::ifstream& file)
{
    clM::RabKar_Strings output;
    size_t count = 0;

    file >> count;
    if (!file.good ())
        throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));
    output.nums_.reserve (count);

    for (size_t i = 0; i < count; ++i)
    {
        size_t size = 0;
        std::string out;

        file >> size;
        if (!file.good ())
            throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

        out.reserve (size);
        for (size_t i = 0; i < size; ++i)
        {
            char elem = 0;
            file >> elem;
            out.push_back (elem);
        }
        if (!file.good ())
            throw std::runtime_error ("Can't proprly read! " + std::string (__FILE__) + std::to_string (__LINE__));

        output.AddString (std::move (out));
    }

    return output;
}
