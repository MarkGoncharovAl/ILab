#pragma once
#include "../MC_OpenCL/MC_OpenCL.hpp"
#include <openssl/sha.h>

namespace clM
{
    class RabKar final : public OpenCL
    {
    public:
        

        RabKar (cl::Device&& Device , const std::string& kernel_file);

        size_t FindPatterns (std::string& base , std::vector<std::string>& patterns);
        void RunEvent (const cl::Kernel& kernel ,
                       const cl::NDRange& loc_sz ,
                       const cl::NDRange& glob_sz) override;

        
    };
}

namespace clMFunc
{
    cl::Buffer GetBuffer (cl::Context& contex , std::string& str);
}