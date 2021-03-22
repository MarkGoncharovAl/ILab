#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <boost/filesystem.hpp>

#include "Common_libs/Errors.hpp"
#include "Common_libs/Time.hpp"

#include "NativeAlg/NativeAlg.hpp"
#include "NativeAlg_GPU/NativeAlg_GPU.hpp"
#include "RabKar/RabKar.hpp"

static std::string ReadBase (std::ifstream& file);
static clM::RabKar_Strings ReadPatterns (std::ifstream& file);
static std::vector<std::ifstream> OpenFilesIn (const std::string& folder);

int main (int argc , char* argv [])
{
    cl::Device device = clM::ChooseDevice (argc , argv);
    if (device == cl::Device {})
        return 0;

    //preparing files
    auto&& files = OpenFilesIn ("../TESTS");
    std::cout << "|Native\t\t|RabKar\t\t|\n";

    for (auto&& file : files)
    {
        std::string base = ReadBase (file);
        auto&& patterns = ReadPatterns (file);
        auto&& patterns_vector = patterns.VecStrings ();

        try
        {
            clM::RabKar rabkar (device);
            MLib::Time time;

            time.Reset ();
            auto&& native = MLib::FindStrings (base , patterns_vector);
            double time_native = time.GetAndResetTime ().count ();
            std::cout << "|" << time_native << "\t\t|";

            time.Reset ();
            auto&& check = rabkar.FindPatterns (base , patterns_vector);
            double time_check = time.GetAndResetTime ().count ();
            std::cout << time_check << "\t\t|";

            bool everything_is_equal = true;
            for (size_t i = 0; i < native.size (); ++i)
            {
                //std::cout << native[i] << std::endl;
                if (check[i] != native[i])
                {
                    std::cout << "\nNot Equal: " << std::to_string (i)
                        << std::endl;
                    std::cout << native[i] << " : " << check[i];
                    everything_is_equal = false;
                }
            }
            if (everything_is_equal)
            {
                if (time_native <= time_check)
                    std::cout << "native in " << time_check / time_native << " times";
                else
                    std::cout << "rabkar in " << time_native / time_check << " times";
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

std::vector<std::ifstream> OpenFilesIn (const std::string& folder_name)
{
    std::vector<std::ifstream> out;

    namespace bfs = boost::filesystem;
    bfs::path folder { folder_name };
    if (!bfs::exists (folder))
    {
        WARNING (std::string ("Can't find folder ") + folder_name);
        return out;
    }

    //folder exists
    bfs::directory_iterator iter(folder), end;
    for (; iter != end; ++iter)
    {
        if (bfs::is_regular_file (*iter))
            out.push_back (std::ifstream { iter->path ().string () });
        //std::cout << iter->path () << std::endl;
    }

    return out;
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
