#include "BSort.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include "../Common_libs/Time/Time.hpp"
#include "../Common_libs/Errors.hpp"
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
    //Checking every platform
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if( platforms.size() == 0)
        throw std::runtime_error("No platforms found. Check OpenCL installation!");

    for (auto&& elem : platforms)
    {
        std::vector<cl::Device> devices;
        elem.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        //if found device -> goooooo
        if ((device_ = clMFunc::FindDevice(devices)) != cl::Device{})
            break;
    }

    context_ = cl::Context{device_};
    queue_ = cl::CommandQueue{context_, device_, CL_QUEUE_PROFILING_ENABLE};

    std::string data_file = clMFunc::ReadFromFile("../BSort/BSort.cl");
    program_ = cl::Program(context_, data_file, true);

    kernel_ = cl::Kernel(program_, "BSort");
}

void clM::Data_t::BSort(std::vector<int> &data, Sort sort)
{
    size_t old_size = clMFunc::PrepareData(data, sort);
    size_t new_size = data.size();
    unsigned numStages = clMFunc::GetNumStages(new_size);

    //buffer is a global memory in kernel
    cl::Buffer buffer(context_, CL_MEM_READ_WRITE, new_size * sizeof(int));
    queue_.enqueueWriteBuffer(buffer, CL_TRUE, 0, new_size * sizeof(int), data.data());

    cl::NDRange global_size = new_size / 2;

    //размер local_size не может превышать размера woek_group
    //поэтому мы можем сранивать элементы только на этом расстоянии
    cl::NDRange local_size = std::min(new_size / 2, device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>());
    //если global_size > local_size, то оставшиеся стадии проходят отдельно
    //т.е. вызывается много раз kernel во ВНЕШНЕМ цикле 
    
    unsigned curStages = std::log2(*local_size);

    MLib::Time time;
    kernel_.setArg(0, buffer);
    kernel_.setArg(1, curStages);
    kernel_.setArg(2, static_cast<unsigned>(sort));
    
    //выделяем локальную память, куда будем писать данные
    //внутри вызова в kernel
    cl::LocalSpaceArg local = cl::Local(2 * (*local_size) * sizeof(int));
    kernel_.setArg(3, local);
    RunEvent(local_size, global_size);

    //обрабатываем стадии, для которых размер work_group > local_size
    //здесь всё медленнее, так как всё время обращаемся только к 
    //глобальной памяти, часто вызываем дорогую операцию RunEvent
    kernel_ = cl::Kernel(program_, "BSort_origin");
    for (; curStages < numStages; ++curStages)
    {
        for (size_t stage_pass = 0; stage_pass < curStages + 1; ++stage_pass)
        {
            kernel_.setArg(0, buffer);
            kernel_.setArg(1, static_cast<unsigned>(curStages));
            kernel_.setArg(2, static_cast<unsigned>(stage_pass));
            kernel_.setArg(3, static_cast<unsigned>(sort));

            RunEvent(local_size, global_size);
        }
    }

    auto time_found = time.Get().count();
    std::cout << MLib::Color::Purple << "Time passed using GPU2: " << time_found << " mc"
              << std::endl
              << MLib::Color::Reset;

    //копируем обратно в исходный массив
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

    WARNING("Can't find device");
    return cl::Device{};
}

unsigned clMFunc::GetNumStages(size_t size)
{
    return std::ceil(std::log2(size));
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