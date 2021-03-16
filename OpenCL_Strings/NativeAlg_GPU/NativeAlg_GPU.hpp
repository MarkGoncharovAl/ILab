#pragma once
#include "../MC_OpenCL/MC_OpenCL.hpp"
#include <unordered_map>
#include <openssl/sha.h>

namespace clM
{
    struct RabKar_Strings
    {
        void AddString(std::string&& str);
        size_t GetLenString(size_t num);
        std::vector<std::string> VecStrings() const;

        std::string data_;
        std::vector<size_t> nums_;
    };

    //private Hash_RabKar - used only for interface 
    class NativeAlg_GPU final : public OpenCL
    {
    public:
        NativeAlg_GPU (const cl::Device& Device);

        std::vector<size_t> FindPatterns (std::string& base ,
                                          RabKar_Strings& patterns);
    
    private:
        void RunEvent (const cl::Kernel& kernel ,
                       const cl::NDRange& loc_sz ,
                       const cl::NDRange& glob_sz) override;
    };
}

namespace clMFunc
{
    cl::Buffer GetBuffer (cl::Context& contex , std::string& str);
}