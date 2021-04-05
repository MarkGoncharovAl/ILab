#pragma once
#include "Errors_DEFINES.hpp"

// level = fatal / error / warning / debug / trace

// Want change directory -> log::ChangeDirectory(string)

// SetFilter -> Set Flag:
//TRACE - everything will be printed
//DEBUG - not trace
//WARNING - warnings errors and fatals - DEFAULT
//ERROR - errors + fatal

#ifndef LOG_LEVEL

#ifdef TRACE
#define LOG_LEVEL 2

#else //Not TRACE
#ifdef DEBUG //Not TRACE
#define LOG_LEVEL 1

#else //Not TRACE + Not DEBUG
#ifdef WARNING //Not TRACE + Not DEBUG
#define LOG_LEVEL 0

#else //Not TRACE + Not DEBUG + Not WARNING
#ifdef ERROR 
#define LOG_LEVEL -1

#else //Default case
#define LOG_LEVEL 0

#endif //ERROR
#endif //WARNING
#endif //DEBUG
#endif //TRACE
#endif //ifdef LOG_LEVEL

#define LOG_fatal if (LOG_LEVEL >= -2) MLib_Error::MAIN_LOG_OBJ.Log<lvl::fatal>(__FILE__, __LINE__)
#define LOG_error if (LOG_LEVEL >= -1) MLib_Error::MAIN_LOG_OBJ.Log<lvl::error>(__FILE__, __LINE__)
#define LOG_warning if (LOG_LEVEL >= 0) MLib_Error::MAIN_LOG_OBJ.Log<lvl::warning>(__FILE__, __LINE__)
#define LOG_debug if (LOG_LEVEL >= 1) MLib_Error::MAIN_LOG_OBJ.Log<lvl::debug>(__FILE__, __LINE__)
#define LOG_trace if (LOG_LEVEL >= 2) MLib_Error::MAIN_LOG_OBJ.Log<lvl::trace>(__FILE__, __LINE__)
