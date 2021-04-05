#pragma once
#include <iostream>
#include <fstream>
#include "../Libs/Errors/Errors.hpp"

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 210
#define CL_HPP_TARGET_OPENCL_VERSION 210
#include <CL/cl2.hpp>

#include <boost/program_options.hpp>

namespace clM
{
    cl::Device ChooseDevice (int agrc , char* argv []);
    cl::Device ChooseFirstDevice();

    //Should be inhereted
    class OpenCL
    {
        public:
        //in order to use device in command line
        OpenCL (int argc , char* argv [] , std::string const& kernel_file);
        //in this constructor device should be prepared
        OpenCL (const cl::Device& device , const std::string& kernel_file);
        //choosing first finding device
        OpenCL (std::string const& kernel_file);

        virtual ~OpenCL () {}

        //Container MUST have:
        //1) size
        //2) data
        //3) front
        template <typename Container>
        cl::Buffer CreateBuffer (Container& cont);

        protected:
        cl::Device device_;
        cl::Context context_;
        cl::Program program_;
        cl::CommandQueue queue_;

        private:
        void ConstructObject (const cl::Device& device , const std::string& kernel_file);
    };
}

namespace clMFunc
{
    using Devices_t = std::vector<std::pair<cl::Platform , cl::Device>>;
    Devices_t GetDevices ();

    void PrintInfoAboutDevice(size_t num, cl::Platform const& platform, cl::Device const& device);

    std::string ReadKernelFile (std::string const& kernel_file);
}

template <typename Container>
cl::Buffer clM::OpenCL::CreateBuffer (Container& cont)
{
    cl::Buffer output (context_ , CL_MEM_READ_WRITE , cont.size () * sizeof (cont.front ()));
    queue_.enqueueWriteBuffer (output , CL_TRUE , 0 , cont.size () * sizeof (cont.front ()) , cont.data ());
    return output;
}