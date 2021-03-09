#pragma once
#include <iostream>
#include <vector>

//для того, чтобы в main проверять на cl::Error
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include <CL/cl2.hpp>

#include <boost/program_options.hpp>

namespace clM
{
    void CheckReturnError (cl_int err , const char* file , size_t line);
#define CheckErr(a) CheckReturnError(a, __FILE__, __LINE__)

    enum class Sort
    {
        Decr = 0 ,
        Incr
    };

    //Binary sort
    void BSort (std::vector<int>& arr , Sort sort = Sort::Incr);
    void BSort (std::vector<int>& arr , const cl::Device& device , Sort sort = Sort::Incr);
    cl::Device GetDevice (int argc , char* argv []);

    class Data_t final
    {
        public:
        Data_t ();
        Data_t (const cl::Device& device);
        void BSort (std::vector<int>& arr , Sort sort);

        private:
        cl::Device device_;
        cl::Context context_;
        cl::Program program_;
        cl::CommandQueue queue_;
        cl::Kernel kernel_;

        void RunEvent (cl::NDRange loc_sz , cl::NDRange glob_sz);
    };

} // namespace clM

namespace clMFunc
{
    using Devices_t = std::vector<std::pair<cl::Platform , cl::Device>>;
    Devices_t GetDevices ();
    cl::Device AnalyseDevices (const Devices_t& devices , int argc , char* argv []);

    cl::Device FindDevice (const std::vector<cl::Device>& devices);
    std::string ReadFromFile (const std::string& file);
    unsigned int GetNumStages (size_t num);

    // returns old_size`
    size_t PrepareData (std::vector<int>& data , clM::Sort sort);
} // namespace clMFunc
