#include "Errors.hpp"
#include <iostream>

int main()
{
    LOG_fatal << "fatal";
    LOG_error << "error";
    LOG_warning << "warning";
    LOG_debug << "debug";
    LOG_trace << "trace";

    LOG::ChangeDirectory("My_debug.log");
    LOG::SetSize_NameFile(13);
    LOG::SetSize_NameLine(2);

    LOG_fatal << "fatality)";
}
