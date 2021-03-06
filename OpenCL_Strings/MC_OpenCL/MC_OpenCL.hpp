#pragma once
#include <iostream>
#include <fstream>
#include "../Common_libs/Errors.hpp"

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include <CL/cl2.hpp>

#include <boost/program_options.hpp>

namespace clM
{
    void CheckReturnError (cl_int err , const char* file , size_t line);
#define CheckErr(a) CheckReturnError(a, __FILE__, __LINE__)

    cl::Device ChooseDevice (int agrc , char* argv []);

    //Should be inhereted
    class OpenCL
    {
    public:
        OpenCL (cl::Device&& device , const std::string& kernel_file);
        virtual ~OpenCL () {}

        virtual void RunEvent (const cl::Kernel& kernel ,
                               const cl::NDRange& loc_sz ,
                               const cl::NDRange& glob_sz) = 0;

    protected:
        cl::Device device_;
        cl::Context context_;
        cl::Program program_;
        cl::CommandQueue queue_;
    };
}

namespace clMFunc
{
    using Devices_t = std::vector<std::pair<cl::Platform , cl::Device>>;
    Devices_t GetDevices ();

    //using for reading from kernel file
    std::string ReadFromFile (const std::string& file);
}