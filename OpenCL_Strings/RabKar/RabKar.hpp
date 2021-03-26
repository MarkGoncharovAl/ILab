#pragma once
#include "../MC_OpenCL/MC_OpenCL.hpp"
#include "Hash_RabKar.hpp"
#include <unordered_map>
#include <chrono>
#include <thread>
#include <mutex>

namespace clM
{
    //private Hash_RabKar - used only for interface 
    class RabKar final : public OpenCL , private Hash_RabKar
    {
    public:
        RabKar (const cl::Device& Device);

        std::vector<size_t> FindPatterns (std::string& base ,
                                          std::vector<std::string>& patterns);

        void HashEffect () const;

    private:
        void RunEvent (const cl::Kernel& kernel ,
                       const cl::NDRange& loc_sz ,
                       const cl::NDRange& glob_sz) override;

        using PrepareHashOfBuffer_t = std::unordered_map < size_t , std::vector<hash_type> >;
        PrepareHashOfBuffer_t hashes;
        cl::Buffer buffer_base;
        std::mutex mutex_;

        void PrepareHashOfBuffer (std::string& base , std::vector<std::string>& patterns);

        //static, because function is called from std::thread -> shoud be static
        static void GetVecHashes (RabKar* pointer , size_t pattern_size , size_t global_size);
    };
}

namespace clMFunc
{
    cl::Buffer GetBuffer (cl::Context& contex , std::string& str);
}