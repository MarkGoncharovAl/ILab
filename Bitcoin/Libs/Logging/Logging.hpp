#pragma once
#include "Logging_DEFINES.hpp"

// level = fatal / error / warning / debug / trace

// Want change directory -> log::ChangeDirectory(string)

// SetFilter -> Set Flag:
//TRACE - everything will be printed
//DEBUG - not trace
//WARNING - warnings errors and fatals - DEFAULT
//ERROR - errors + fatal


#define LOG_fatal MLib_Error::MAIN_LOG_OBJ.Log<lvl::fatal>(__FILE__, __LINE__)
#define LOG_error MLib_Error::MAIN_LOG_OBJ.Log<lvl::error>(__FILE__, __LINE__)

#ifdef TRACE
#define LOG_trace MLib_Error::MAIN_LOG_OBJ.Log<lvl::trace>(__FILE__, __LINE__)
#define LOG_debug MLib_Error::MAIN_LOG_OBJ.Log<lvl::debug>(__FILE__, __LINE__)
#define LOG_warning MLib_Error::MAIN_LOG_OBJ.Log<lvl::warning>(__FILE__, __LINE__)

#else //Not TRACE
#define LOG_trace MLib_Error::FAKE_LOGGING

#ifdef DEBUG //Not TRACE
#define LOG_debug MLib_Error::MAIN_LOG_OBJ.Log<lvl::debug>(__FILE__, __LINE__)
#define LOG_warning MLib_Error::MAIN_LOG_OBJ.Log<lvl::warning>(__FILE__, __LINE__)

#else //Not TRACE + Not DEBUG
#define LOG_debug MLib_Error::FAKE_LOGGING

#ifdef WARNING //Not TRACE + Not DEBUG
#define LOG_warning MLib_Error::MAIN_LOG_OBJ.Log<lvl::warning>(__FILE__, __LINE__)

#else //Not TRACE + Not DEBUG + Not WARNING
#ifdef ERROR 
#define LOG_warning MLib_Error::FAKE_LOGGING

#else //Default case
#define LOG_warning MLib_Error::MAIN_LOG_OBJ.Log<lvl::warning>(__FILE__, __LINE__)

#endif //ERROR
#endif //WARNING
#endif //DEBUG
#endif //TRACE