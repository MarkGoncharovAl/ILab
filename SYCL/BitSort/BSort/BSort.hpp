#pragma once
#include <iostream>
#include <vector>

#include "../MySycl/MySycl.hpp"

namespace Msycl
{
    class BSort : private MySycl
    {
    public:
        BSort ();
        enum class Sort
        {
            Decr = 0 ,
            Incr
        };

        void sort (std::vector<int>& arr , Sort sort = Sort::Incr);

    private:
        unsigned GetNumStages(size_t size) const;

        //returns old size of data
        size_t PrepareData(std::vector<int>& data, Sort sort) const;
    };
} // namespace clM

// namespace clMFunc
// {
//     using Devices_t = std::vector<std::pair<cl::Platform , cl::Device>>;
//     Devices_t GetDevices ();
//     cl::Device AnalyseDevices (const Devices_t& devices , int argc , char* argv []);

//     cl::Device FindDevice (const std::vector<cl::Device>& devices);
//     std::string ReadFromFile (const std::string& file);
//     unsigned int GetNumStages (size_t num);

//     // returns old_size`
//     size_t PrepareData (std::vector<int>& data , clM::Sort sort);
// } // namespace clMFunc
