#include "RabKar.hpp"

clM::RabKar::RabKar (cl::Device&& device)
    : OpenCL (std::move (device) , "../RabKar/RabKar.cl")
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
        cl::NDRange local_size = 1;

        //creating hashes for base string
        std::vector<hash_type> hash_buffer (*global_size);
        cl::Buffer buffer_hash (context_ , CL_MEM_READ_WRITE , hash_buffer.size () * sizeof (hash_type));
        queue_.enqueueWriteBuffer (buffer_hash , CL_TRUE , 0 , hash_buffer.size () * sizeof (hash_type), hash_buffer.data ());

        //writting hashes into array
        kernel.setArg (0 , buffer_base);
        kernel.setArg (1, buffer_hash);
        kernel.setArg (2, patterns[i].size ());
        RunEvent(kernel, local_size, global_size);

        //getting main number
        hash_type hash_pattern = HashFunction(patterns[i]);
        //std::cout << "My hash: " << hash_pattern << std::endl;

        size_t count = 0;
        hash_type* check_buffer = (hash_type*) queue_.enqueueMapBuffer(buffer_hash, CL_TRUE, CL_MAP_READ, 0, hash_buffer.size () * sizeof (hash_type));
        //comparing hashes
        for (size_t j = 0; j < *global_size; ++j)
        {
            //std::cout << "Checking hash: " << check_buffer[j] << std::endl;
            if (check_buffer[j] == hash_pattern)
                //if (LastCompare(patterns[i].c_str(), base.c_str() + j, patterns[i].size()))
                count++; //found
        }

        queue_.enqueueUnmapMemObject(buffer_hash, check_buffer);
        output.emplace_back(count);
    }

    //std::cout << "Finding: " << Findings() << "%" << std::endl;
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







