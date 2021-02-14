#pragma once

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

typedef int fd_t;
typedef struct 
{
    fd_t in_;
    fd_t out_;
} pipe_t;

void errror(char info[], size_t LINE, const char *FILE);

#define ERROR(a) errror(a, __LINE__, __FILE__)

pid_t fork_safe(int LINE);
void pipe_safe(pipe_t* data, int LINE);
void dup2_safe(fd_t old_fd, fd_t new_fd, int LINE);

void write_safe(fd_t fd, void *buf, size_t size, int LINE);
void read_safe(fd_t fd, void *buf, size_t size, int LINE);

fd_t open_safe(const char *file, int O_FLAG, int LINE);

char *mmap_safe(fd_t file, int LINE);

#define pipe_s(a) pipe_safe(a, __LINE__)
#define fork_s() fork_safe(__LINE__)
#define open_s(a, b) open_safe(a, b, __LINE__)
#define mmap_s(a) mmap_safe(a, __LINE__)
#define write_s(a, b, c) write_safe(a, b, c, __LINE__);
#define read_s(a, b, c) read_safe(a, b, c, __LINE__);
#define dup2_s(a, b) dup2_safe(a, b, __LINE__)
