#include "MarkAlg.hpp"

clM::MarkAlg::MarkAlg (const cl::Device& device)
    : OpenCL (device , "../MarkAlg/MarkAlg.cl")
{}


std::vector<size_t> clM::MarkAlg::FindPatterns
(std::string& base , std::vector<std::string>& patterns)
{
    std::vector<size_t> output (patterns.size ());

    cl::Buffer buffer_base = CreateBuffer (base);

    //preparing hashe table that will be compared
    auto&& hash = PrepareHashOfBuffer (base.size () , buffer_base);

    auto&& out_buffer = CreateBuffer (output);
    auto&& findings = PrepareFindings (buffer_base , hash.first , out_buffer , patterns);
    return findings;

    for (std::string& str : patterns)
    {

    }

    return output;
}

void clM::MarkAlg::RunEvent (const cl::Kernel& kernel ,
                            const cl::NDRange& loc_sz ,
                            const cl::NDRange& glob_sz)
{
    cl::Event event;
    queue_.enqueueNDRangeKernel (kernel , 0 , glob_sz , loc_sz , nullptr , &event);
    event.wait ();
}

std::pair<cl::Buffer , std::vector<unsigned long>>
clM::MarkAlg::PrepareHashOfBuffer (size_t base_size , cl::Buffer& buffer_base)
{
    cl::Kernel kernel (program_ , "PrepareHashOfBuffer");
    cl::NDRange local_size = 1;

    constexpr char part_size = 8; //in hash will be 'part_size' bytes

    cl::NDRange global_size = base_size - part_size + 1;

    //creating hashes for base string
    std::vector<hash_type> hash_buffer (*global_size);

    //creating buffers for output hashes
    auto&& cur_buffer = CreateBuffer (hash_buffer);

    //writting hashes into array
    kernel.setArg (0 , buffer_base);
    kernel.setArg (1 , cur_buffer);
    kernel.setArg (2 , part_size);
    RunEvent (kernel , local_size , global_size);

    //cheking hashes
    cl::copy (queue_ , cur_buffer , hash_buffer.begin () , hash_buffer.end ());
    return std::make_pair (cur_buffer , hash_buffer);
}

std::vector<size_t>
clM::MarkAlg::PrepareFindings (cl::Buffer& base , cl::Buffer& hash_base, std::vector<std::string>& patterns)
{
    cl::ImageFormat format(CL_RGBA, CL_UNSIGNED_INT32);
    cl::Image2D cl_signature_table(context_, CL_MEM_READ_ONLY | CL_MEM_HOST_PTR
                                    , format, , patterns.size ());

    for (auto&& str : patterns)
    {

    }

    for (auto&& str : patterns)
    {
        //getting main number
        hash_type hash_pattern = HashFunction (str);

        //getting current hash table
        auto&& base_hashes = hashes.find (str.size ());
        if (base_hashes == hashes.end ())
        {
            WARNING ("Hash " + std::to_string (str.size ()) + " wasn't prepared!");
            continue;
        }

        auto& check = base_hashes->second;
        size_t count = 0;

        //comparing hashes
        for (size_t i = 0; i < check.size (); ++i)
        {
            if (check[i] == hash_pattern)
                if (LastCompare (str.data () , base.data () + i , str.size ()))
                    count++;
        }

        output.emplace_back (count);
    }
}