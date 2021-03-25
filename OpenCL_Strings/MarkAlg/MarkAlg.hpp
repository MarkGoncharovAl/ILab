#pragma once
#include "../MC_OpenCL/MC_OpenCL.hpp"
#include "Hash_RabKar.hpp"
#include <unordered_map>

namespace clM
{
    //private Hash_RabKar - used only for interface 
    class MarkAlg final : public OpenCL , private Hash_RabKar
    {
    public:
        MarkAlg (const cl::Device& Device);

        std::vector<size_t> FindPatterns (std::string& base ,
                                          std::vector<std::string>& patterns);
        
    
    private:
        using hash_type = size_t;

        void RunEvent (const cl::Kernel& kernel ,
                       const cl::NDRange& loc_sz ,
                       const cl::NDRange& glob_sz) override;


        std::pair<cl::Buffer, std::vector<size_t>>
        PrepareHashOfBuffer(size_t base, cl::Buffer& buffer_base);

        std::vector<size_t>
        PrepareFindings(cl::Buffer& base, cl::Buffer& hash_base, std::vector<std::string>& patterns);
    };
}

namespace clMFunc
{
    cl::Buffer GetBuffer (cl::Context& contex , std::string& str);
}