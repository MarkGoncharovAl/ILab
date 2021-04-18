#include "NativeAlg.hpp"


size_t MLib::FindStrings(const std::string& base, const std::string& pattern)
{
    const char* pat = pattern.c_str();
    const char* bas = base.c_str();
    const char* last = base.size() + bas;

    size_t size = pattern.size();
    size_t findings = 0;

    for (; bas != last; ++bas)
    {   
        bool found = true;
        for (size_t i = 0; i < size; ++i)
        {
            if (pat[i] != bas[i])
            {
                found = false;
                break;
            }
        }
        if (found)
            findings++;
    }
    return findings;
}

std::vector<size_t> MLib::FindStrings(const std::string& base, const std::vector<std::string>& patterns)
{
    std::vector<size_t> output;
    output.reserve(patterns.size());

    for (const std::string& str : patterns)
        output.emplace_back(FindStrings(base, str));

    return output;
}