#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cmath>
#include <boost/filesystem.hpp>

#include "Common_libs/Time.hpp"
#include "Common_libs/Errors/Errors.hpp"
#include "Common_libs/Table.hpp"

#include "NativeAlg/NativeAlg.hpp"
#include "NativeAlg_GPU/NativeAlg_GPU.hpp"
#include "RabKar/RabKar.hpp"

using CUR_TABLE = MLib::Table_h<double , 6>;

static std::string ReadBase (std::ifstream& file);
static clM::RabKar_Strings ReadPatterns (std::ifstream& file);
static std::vector<std::ifstream> OpenFilesIn (const std::string& folder);
static bool IsExtensionTxt (const boost::filesystem::path& path);
static CUR_TABLE PrepareTable ();
static void AddToTable (CUR_TABLE& table , const clM::RabKar& alg , double native , double rabkar);

int main (int argc , char* argv [])
{
    cl::Device device = clM::ChooseDevice (argc , argv);
    if (device == cl::Device {})
        return 0;

    //preparing files
    auto&& files = OpenFilesIn ("../TESTS");
    auto&& table = PrepareTable ();

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

            time.Reset ();
            auto&& check = rabkar.FindPatterns (base , patterns_vector);
            double time_check = time.GetAndResetTime ().count ();

            AddToTable (table , rabkar , time_native , time_check);

            for (size_t i = 0; i < native.size (); ++i)
            {
                //std::cout << native[i] << std::endl;
                if (check[i] != native[i])
                {
                    std::cout << "\nNot Equal: " << std::to_string (i)
                        << std::endl;
                    std::cout << native[i] << " : " << check[i];
                    std::cout << std::endl;
                }
            }

            file.close ();
        }
        catch (cl::Error& err)
        {
            LOG_fatal << err.what ();
            clM::CheckReturnError (err.err ());
        }
        catch (std::exception& err)
        {
            LOG_fatal << err.what ();
        }

    }
    table.Print ();
    return 0;
}

std::vector<std::ifstream> OpenFilesIn (const std::string& folder_name)
{
    std::vector<std::ifstream> out;

    namespace bfs = boost::filesystem;
    bfs::path folder { folder_name };
    if (!bfs::exists (folder))
    {
        LOG_warning << "Can't find folder " << folder_name;
        return out;
    }

    //folder exists
    bfs::directory_iterator iter (folder) , end;
    for (; iter != end; ++iter)
    {
        if (bfs::is_regular_file (*iter)
        && IsExtensionTxt (iter->path ()))
        {
            out.push_back (std::ifstream { iter->path ().string () });
        }
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

bool IsExtensionTxt (const boost::filesystem::path& path)
{
    return (boost::filesystem::extension (path) == ".txt");
}

CUR_TABLE PrepareTable ()
{
    CUR_TABLE table;
    table.spaces_ = 10;
    table.header_[0] = "NATIVE";
    table.header_[1] = "PREPARING";
    table.header_[2] = "GPU_TIME";
    table.header_[3] = "COMPARING";
    table.header_[4] = "FASTER";
    table.header_[5] = "TIMES";
    return table;
}

void AddToTable (CUR_TABLE& table , const clM::RabKar& alg , double native , double rabkar)
{
    double faster = (native < rabkar) ? 0 : 1;
    double faster_times = (native > rabkar)
        ? native / rabkar
        : rabkar / native;

    table.data_.push_back (std::array<double , 6>
    {       native ,
            rabkar - alg.GetKernelTime () - alg.GetCompareTime () ,
            alg.GetKernelTime () ,
            alg.GetCompareTime () ,
            faster ,
            faster_times
    });
}