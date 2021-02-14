#include "../BSort/BSort.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "../Common_libs/Errors/Errors.hpp"
#include "../Common_libs/Color.hpp"
#include "../Common_libs/Random/Random.hpp"
#include "../Common_libs/Time/Time.hpp"

static std::vector<int> CreateTest();
static void CompareData(const std::vector<int> &data, const std::vector<int> &data_check);

int main()
{
    constexpr size_t size = 9;
    std::array<std::ifstream, size> files = {
        std::ifstream{"../TESTS/1.txt"},
        std::ifstream{"../TESTS/2.txt"},
        std::ifstream{"../TESTS/3.txt"},
        std::ifstream{"../TESTS/4.txt"},
        std::ifstream{"../TESTS/5.txt"},
        std::ifstream{"../TESTS/6.txt"},
        std::ifstream{"../TESTS/7.txt"},
        std::ifstream{"../TESTS/8.txt"},
        std::ifstream{"../TESTS/9.txt"},
    };

    for (size_t i = 0; i < size; ++i)
    {
        std::vector<int> data = CreateTest();
        std::vector<int> data_check = data;

        MLib::Time time;
        std::sort(data_check.begin(), data_check.end());
        auto time_found = time.Get().count();
        std::cout << MLib::Color::Purple << "Time passed using std::sort: " << time_found << " mc"
                  << std::endl
                  << MLib::Color::Reset;

        try
        {
            clM::BSort(data);
            CompareData(data, data_check);
        }
        catch (cl::Error &err)
        {
            std::cerr << err.what() << std::endl;
            clM::CheckErr(err.err());
        }
        catch (std::exception &err)
        {
            std::cerr << err.what() << std::endl;
        }
    }
    return 0;
}

std::vector<int> CreateTest()
{
    constexpr size_t size = 10000;
    MLib::Random rand{0, 2000};

    std::vector<int> out;
    out.reserve(size);

    for (size_t i = 0; i < size; ++i)
        out.push_back(rand.get());

    return out;
}

void CompareData(const std::vector<int> &data, const std::vector<int> &data_check)
{
    if (data.size() != data_check.size())
    {
        std::cout << MLib::Color::Bold_Red << "Arrays can't be compaired!\n";
        return;
    }

    for (size_t i = 0; i < data.size(); ++i)
    {
        if (data[i] != data_check[i])
        {
            for (size_t j = 0; j < 20; ++j)
                std::cout << data[j] << std::endl;
            std::cout << MLib::Color::Bold_Red << "Not equal arrays!\n";
            return;
        }
    }

    std::cout << MLib::Color::Green << "Equal arrays!\n";
}