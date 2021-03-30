#pragma once
#include "../MC_OpenCL/MC_OpenCL.hpp"
#include "Hash_RabKar.hpp"
#include <unordered_map>
#include <chrono>
#include <thread>
#include <queue>

namespace clM
{
    //private Hash_RabKar - used only for interface 
    class RabKar final : public OpenCL , private Hash_RabKar
    {
    private:

        /*
        1) Structer for not waiting every event
        2) It has neccessary opportunity to copying data from GPU
        3) Because hash_ has reference -> hash data are going to the
        unordered_map, that's called hases
        */
        class EventWaiter
        {
        private:
            std::vector<hash_type>& hash_;
            cl::Buffer buffer_;
            cl::Event event_;
        public:
            EventWaiter (std::vector<hash_type>& hash , cl::Buffer buffer);
            cl::Event* GetEvent () noexcept { return &(event_); }
            
            //return 
            double CopyDataFromGPU (cl::CommandQueue& queue);
        };

        std::queue<EventWaiter> event_data_;

        cl::Buffer buffer_base_;
        double gpu_time_ = 0;
        double compare_ = 0;

        using PrepareHashOfBuffer_t = std::unordered_map < size_t , std::vector<hash_type> >;
        PrepareHashOfBuffer_t
            PrepareHashOfBuffer (std::string& base , std::vector<std::string>& patterns);


        void GetVecHashes (size_t pattern_size , size_t global_size , std::vector<hash_type>& hash_buffer);
        void CopyDataFromGPU ();


    public:
        RabKar (const cl::Device& Device);

        std::vector<size_t> FindPatterns (std::string& base ,
                                          std::vector<std::string>& patterns);

        void HashEffect () const;
        double GetKernelTime () const noexcept { return gpu_time_; }
        double GetCompareTime () const noexcept { return compare_; }
    };
}

namespace clMFunc
{
    cl::Buffer GetBuffer (cl::Context& contex , std::string& str);
}