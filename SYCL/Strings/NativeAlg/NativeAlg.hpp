#pragma once

#include <iostream>
#include <vector>

namespace MLib
{
    size_t FindStrings(const std::string& base, const std::string& pattern);
    std::vector<size_t> FindStrings(const std::string& base, const std::vector<std::string>& patterns);
}