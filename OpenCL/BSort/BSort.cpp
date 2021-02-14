#include "BSort.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include "../Common_libs/Time/Time.hpp"
#include "../Common_libs/Errors/Errors.hpp"
#include "../Common_libs/Color.hpp"

void clM::CheckReturnError(cl_int err, const char *file, size_t line)
{
    switch (err)
    {
    case CL_SUCCESS:
        break;
    case CL_INVALID_VALUE:
        MLib::print_error("INVALID VALUE", file, line);
        break;
    case CL_INVALID_DEVICE:
        MLib::print_error("INVALID DEVICE", file, line);
        break;
    case CL_OUT_OF_HOST_MEMORY:
        MLib::print_error("OUT_OF_HOST_MEMORY", file, line);
        break;
    case CL_OUT_OF_RESOURCES:
        MLib::print_error("OUT_OF_RESOURCES", file, line);
        break;
    default:
        MLib::print_error("WEIRD MISTAKE IN CHECKING ERROR", file, line);
        break;
    }
}

void clM::BSort(std::vector<int> &data, Sort sort /* = Sort::Incr*/)
{
    // при создании Data_t
    // уже готовятся основные переменные
    Data_t{}.BSort(data, sort);
}

clM::Data_t::Data_t()
{
    std::vector<cl::Device> devices;
    cl::Platform{}.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    device_ = clMFunc::FindDevice(devices);

    context_ = cl::Context{device_};
    queue_ = cl::CommandQueue{context_, device_, CL_QUEUE_PROFILING_ENABLE};

    std::string data_file = clMFunc::ReadFromFile("../BSort.cl");
    program_ = cl::Program(context_, data_file, true);

    kernel_ = cl::Kernel(program_, "BSort");
}

void clM::Data_t::BSort(std::vector<int> &data, Sort sort)
{
    size_t old_size = clMFunc::PrepareData(data, sort);
    size_t new_size = data.size();

    cl::Buffer buffer(context_, CL_MEM_READ_WRITE, new_size * sizeof(int));
    queue_.enqueueWriteBuffer(buffer, CL_TRUE, 0, new_size * sizeof(int), data.data());

    cl::NDRange global_size = new_size / 2;
    unsigned numStages = std::round(std::log2(new_size));
    cl::NDRange local_size = std::min(new_size / 2 , device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>());

    {
        MLib::Time time;
        kernel_.setArg(0, buffer);
        kernel_.setArg(1, numStages);
        kernel_.setArg(2, static_cast<unsigned>(sort));

        RunEvent(local_size, global_size);
        auto time_found = time.Get().count();
        std::cout << MLib::Color::Purple << "Time passed using GPU1: " << time_found << " mc"
                  << std::endl
                  << MLib::Color::Reset;
    }
    // {
    //     /*second part*/
    //     MLib::Time time;
    //     kernel_ = cl::Kernel(program_, "BSort_origin");
    //     for (; stage < numStages; ++stage)
    //     {
    //         for (std::size_t stage_pass = 0; stage_pass < stage + 1; ++stage_pass)
    //         {
    //             kernel_.setArg(0, buffer);
    //             kernel_.setArg(1, static_cast<unsigned>(stage));
    //             kernel_.setArg(2, static_cast<unsigned>(stage_pass));
    //             kernel_.setArg(3, static_cast<unsigned>(sort));

    //             RunEvent(local_size, global_size);
    //         }
    //     }
    //     auto time_found = time.Get().count();
    //     std::cout << MLib::Color::Purple << "Time passed using GPU2: " << time_found << " mc"
    //               << std::endl
    //               << MLib::Color::Reset;
    // }

    auto map_data = (int *)queue_.enqueueMapBuffer(buffer, CL_TRUE, CL_MAP_READ, 0, new_size * sizeof(int));
    for (size_t i = 0; i < new_size; i++)
        data[i] = map_data[i];
    queue_.enqueueUnmapMemObject(buffer, map_data);

    data.resize(old_size);
}

void clM::Data_t::RunEvent(cl::NDRange loc_sz, cl::NDRange glob_sz)
{
    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel_, 0, glob_sz, loc_sz, nullptr, &event);
    event.wait();
}

std::string clMFunc::ReadFromFile(const std::string &file)
{
    std::ifstream file_stream(file);
    if (!file_stream.is_open())
        throw std::runtime_error("Can't find file: " + file);

    return std::string{std::istreambuf_iterator<char>{file_stream}, std::istreambuf_iterator<char>{}};
}

cl::Device clMFunc::FindDevice(const std::vector<cl::Device> &devices)
{
    for (const auto &device : devices)
    {
        if (device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>())
            return device;
    }

    throw std::runtime_error("Can't find device");
}

size_t clMFunc::PrepareData(std::vector<int> &data, clM::Sort sort)
{
    size_t old_size = data.size();
    size_t new_size = 1;
    while (new_size <= old_size)
        new_size *= 2;
    //std::cout << new_size;

    int pushing_num = 0;
    if (sort == clM::Sort::Incr)
        pushing_num = std::numeric_limits<int>::max();
    else
        pushing_num = std::numeric_limits<int>::min();

    data.reserve(new_size);
    for (size_t i = old_size; i < new_size; ++i)
        data.push_back(pushing_num);

    return old_size;
}