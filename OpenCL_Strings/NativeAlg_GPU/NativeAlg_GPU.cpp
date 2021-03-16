#include "NativeAlg_GPU.hpp"

void clM::RabKar_Strings::AddString (std::string&& str)
{
    data_ += str;
    nums_.push_back (data_.size ());
}

std::vector<std::string> clM::RabKar_Strings::VecStrings() const
{
    std::vector<std::string> output;
    output.reserve (nums_.size ());
    auto&& pushing = std::back_inserter(output);

    auto&& start_string = data_.begin();
    auto&& start = data_.begin ();

    for (size_t elem : nums_)
    {
        auto end = start_string + elem;
        pushing = {start, end};
        start = end;
    }
    return output;
}

clM::NativeAlg_GPU::NativeAlg_GPU (const cl::Device& device)
    : OpenCL (device , "../NativeAlg_GPU/NativeAlg_GPU.cl")
{}

std::vector<size_t> clM::NativeAlg_GPU::FindPatterns
(std::string& base , RabKar_Strings& patterns)
{
    // std::cout << "Base: " << base;
    // std::cout << "\nPattern: " << patterns.data_ << std::endl;
    // for (size_t elem : patterns.nums_)
    //     std::cout << elem << "\t";
    // std::cout << "\n";

    cl::Buffer buffer_base = CreateBuffer (base);

    std::vector<size_t> output(patterns.nums_.size(), 0);
    cl::Buffer buffer_patterns = CreateBuffer (patterns.data_);
    cl::Buffer nums_buffer = CreateBuffer (patterns.nums_);
    cl::copy(queue_, patterns.nums_.begin(), patterns.nums_.end(), nums_buffer);
    cl::Buffer output_buffer = CreateBuffer (output);

    cl::Kernel kernel (program_ , "CheckHashes");
    kernel.setArg (0 , buffer_base);
    kernel.setArg (1 , base.size ());
    kernel.setArg (2 , buffer_patterns);
    kernel.setArg (3 , nums_buffer);
    kernel.setArg (4 , output_buffer);
    RunEvent (kernel , 1 , patterns.nums_.size ());

    cl::copy (queue_ , output_buffer , output.begin () , output.end ());
    return output;
}

void clM::NativeAlg_GPU::RunEvent (const cl::Kernel& kernel ,
                           const cl::NDRange& loc_sz ,
                           const cl::NDRange& glob_sz)
{
    cl::Event event;
    queue_.enqueueNDRangeKernel (kernel , 0 , glob_sz , loc_sz , nullptr , &event);
    event.wait ();
}