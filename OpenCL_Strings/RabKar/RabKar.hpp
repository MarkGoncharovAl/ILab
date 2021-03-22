#pragma once
#include "../MC_OpenCL/MC_OpenCL.hpp"
#include "Hash_RabKar.hpp"
#include <unordered_map>

namespace clM
{
    //private Hash_RabKar - used only for interface 
    class RabKar final : public OpenCL , private Hash_RabKar
    {
    public:
        RabKar (const cl::Device& Device);

        std::vector<size_t> FindPatterns (std::string& base ,
                                          std::vector<std::string>& patterns);
        
        void HashEffect() const;
    
    private:
        void RunEvent (const cl::Kernel& kernel ,
                       const cl::NDRange& loc_sz ,
                       const cl::NDRange& glob_sz) override;

        std::unordered_map 
        < size_t , std::vector<hash_type> >
        PrepareHashOfBuffer(std::string& base , std::vector<std::string>& patterns, cl::Buffer& buffer_base);
    };
}

namespace clMFunc
{
    cl::Buffer GetBuffer (cl::Context& contex , std::string& str);
}