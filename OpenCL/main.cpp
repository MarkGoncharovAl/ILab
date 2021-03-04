#include "BSort/BSort.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "Common_libs/Errors.hpp"

static std::vector<int> ReadData();
static void WriteData(const std::vector<int> &data);

int main(int argc, char* argv[])
{
    cl::Device device = clM::GetDevice(argc, argv);
    if (device == cl::Device{})
        return 0;

    std::vector<int> data = ReadData();

    try
    {
        clM::BSort(data, device, clM::Sort::Decr);
        WriteData(data);
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

    return 0;
}

std::vector<int> ReadData()
{
    int amount = 0;
    std::cin >> amount;
    if (!std::cin.good())
        ERROR("Can't read the amount of elements!");

    std::vector<int> data;
    for (int i = 0; i < amount; ++i)
    {
        int new_num = 0;
        std::cin >> new_num;
        if (!std::cin.good())
            ERROR("Can't read the element!");

        data.push_back(new_num);
    }

    return data;
}

void WriteData(const std::vector<int> &data)
{
    std::cout << "\nFinal data:\n";
    for (int elem : data)
    {
        std::cout << elem << '\t';
        if (!std::cout.good())
            ERROR("Can't print answer!");
    }
    std::cout << std::endl;
}