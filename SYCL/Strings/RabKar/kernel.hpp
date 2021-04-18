#pragma once
#include <iostream>

static const std::string kernel_file = "\
__kernel void PrepareHashOfBuffer (__global char* buffer , __global unsigned* hashes , ulong size)\
{\
    uint id = get_global_id (0);\
\
    ulong hash = 1;\
    char* str = buffer + id;\
\
    for (ulong i = 0; i < size; ++i)\
    {\
        hash = ((hash * 31) + *str) % (1 << 25);\
        str++;\
    }\
\
    hashes[id] = hash;\
}\
";