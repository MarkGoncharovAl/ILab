#pragma once

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

//default out is STDOUT_FILENO

enum {LOG_INFO = 0, LOG_ERROR, LOG_WARNING};

int SetLogFile(int fd);
int UnSetLogFile();

int WriteLoggingFile(int level, char* message, ...);

#define pr_log(level, str, ...) WriteLoggingFile(level, str" FILE:%s LINE:%d",  ##__VA_ARGS__, __FILE__, __LINE__)

#define pr_err(str, ...)  pr_log(LOG_ERROR, str, ##__VA_ARGS__);
#define pr_strerr(str, ...)  pr_log(LOG_ERROR, str" ERRNO:%d STRERR:%s",  ##__VA_ARGS__, errno, strerror(errno));
#define pr_war(str, ...)  pr_log(LOG_WARNING, str, ##__VA_ARGS__);

#ifdef INFO
    #define pr_info(str, ...) pr_log(LOG_INFO, str, ##__VA_ARGS__);
#else
    #define pr_info(str, ...)
#endif //CLOSE_INFO
