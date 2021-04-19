#include <Logging.hpp>
#include <iostream>

//Compiling with flag -D LOG_FILE -D TRACE

int main()
{
    //default is "debug.log"
    LOG::ChangeDirectory("Examples3.log"); 

    LOG::SetSize_NameFile(13); //set length of string of FILE 13
    LOG::SetSize_NameLine(2);  //set length of string of LINE 2 

    LOG_fatal << "fatal_message";
    LOG_error << "error_message";
    LOG_warning << "warning_message";
    LOG_debug << "debug_message";
    LOG_trace << "trace_message";
}
