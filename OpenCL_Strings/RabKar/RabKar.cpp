#include "RabKar.hpp"

clM::RabKar::RabKar (cl::Device&& device , const std::string& kernel_file)
    : OpenCL (std::move (device) , kernel_file)
{}

std::vector<size_t> clM::RabKar::FindPatterns
(std::string& base , std::vector<std::string>& patterns)
{
    std::vector<size_t> output;
    output.reserve(patterns.size ());

    cl::Kernel kernel (program_ , "RabKar");
    cl::Buffer buffer_base (context_ , CL_MEM_READ_WRITE , base.size ());
    queue_.enqueueWriteBuffer (buffer_base , CL_TRUE , 0 , base.size () , base.data ());

    for (size_t i = 0; i < patterns.size (); ++i)
    {
        cl::NDRange global_size = base.size () - patterns[i].size () + 1;
        cl::NDRange local_size = std::min (*global_size , device_.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE> ());

        //creating hashes for base string
        std::vector<hash_type> hash_buffer (*global_size);
        cl::Buffer buffer_hash (context_ , CL_MEM_READ_WRITE , hash_buffer.size ());
        queue_.enqueueWriteBuffer (buffer_hash , CL_TRUE , 0 , hash_buffer.size () , hash_buffer.data ());

        //writting hashes into array
        kernel.setArg (0 , buffer_base);
        kernel.setArg (1, buffer_hash);
        kernel.setArg (2, static_cast<unsigned>(patterns[i].size ()));
        RunEvent(kernel, local_size, global_size);

        //getting main number
        hash_type hash_pattern = HashFunction(patterns[i]);

        size_t count = 0;
        //comparing hashes
        for (size_t j = 0; j < base.size (); ++j)
        {
            if (hash_buffer[i] == hash_pattern)
                if (LastCompare(patterns[i].c_str(), base.c_str() + j, patterns[i].size()))
                    count++; //found
        }
        output.emplace_back(count);
    }

    return output;
}

void clM::RabKar::RunEvent(const cl::Kernel& kernel, 
                           const cl::NDRange& loc_sz, 
                           const cl::NDRange& glob_sz)
{
    cl::Event event;
    queue_.enqueueNDRangeKernel(kernel, 0, glob_sz, loc_sz, nullptr, &event);
    event.wait();
}







