#pragma once

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

//  !DON'T FORGET
//  !1) if you use shared memory you've got to have
//  !   file SHARED_MEMORY

typedef char bool_t;
#define true 1
#define false 0

typedef int fd_t;

typedef int sem_t;
#define MY_IPC_ERROR -1
#define FILE_SHARED_MEM "/home/alexandr/VS_prog/Common_libs/SHARED_MEMORY"

typedef struct
{
    fd_t in_;
    fd_t out_;
} pipe_t;

//  !Checking errors
//////////////////////////////////////////////
void errror(char info[], size_t LINE, const char *FILE);
void warrning(char info[], size_t LINE, const char *FILE);
#define ERROR(a) errror(a, __LINE__, __FILE__)
#define WARNING(a) warrning(a, __LINE__, __FILE__)
//////////////////////////////////////////////

pid_t fork_safe(int LINE, const char *FILE);
void pipe_safe(pipe_t *data, int LINE, const char *FILE);
void dup2_safe(fd_t old_fd, fd_t new_fd, int LINE, const char *FILE);

void write_safe(fd_t fd, void *buf, size_t size, int LINE, const char *FILE);
void read_safe(fd_t fd, void *buf, size_t size, int LINE, const char *FILE);

fd_t open_safe(const char *file, int O_FLAG, int LINE, const char *FILE);

void *mmap_safe(fd_t file, int LINE, const char *FILE);

//  !shared memory!
//////////////////////////////////////////////
void *get_shared_memory(int size, int key, int LINE, const char *file);
int detach_shared_memory(void *block, int LINE, const char *file);
int destroy_shared_memory(int key, int LINE, const char *file);
//////////////////////////////////////////////

//  !Semaphors
//////////////////////////////////////////////
sem_t sem_get(int key);
void sem_increase(sem_t sem);
void sem_decrease(sem_t sem);
//////////////////////////////////////////////

#define pipe_s(a) pipe_safe(a, __LINE__, __FILE__)
#define fork_s() fork_safe(__LINE__, __FILE__)
#define open_s(a, b) open_safe(a, b, __LINE__, __FILE__)
#define mmap_s(a) mmap_safe(a, __LINE__, __FILE__)
#define write_s(a, b, c) write_safe(a, b, c, __LINE__, __FILE__)
#define read_s(a, b, c) read_safe(a, b, c, __LINE__, __FILE__)
#define dup2_s(a, b) dup2_safe(a, b, __LINE__, __FILE__)
#define get_shared_memory_s(a, b) get_shared_memory(a, b, __LINE__, __FILE__)
#define detach_shared_memory_s(a) detach_shared_memory(a, __LINE__, __FILE__)
#define destroy_shared_memory_s(a) destroy_shared_memory(a, __LINE__, __FILE__)

//  !REALIZATIONS
//////////////////////////////////////////////
void pipe_safe(pipe_t *data, int LINE, const char *FILE)
{
    int data_pipe[2] = {};
    int out_pipe = pipe(data_pipe);
    if (out_pipe == -1)
        errror("PIPE!", LINE, FILE);

    data->in_ = data_pipe[0];
    data->out_ = data_pipe[1];
}

pid_t fork_safe(int LINE, const char *FILE)
{
    pid_t out = fork();
    if (out == -1)
        errror("FORK!", LINE, FILE);

    return out;
}

fd_t open_safe(const char *file, int O_FLAG, int LINE, const char *FILE)
{
    fd_t output = open(file, O_FLAG);

    struct stat st;
    fstat(output, &st);

    if (output < 0 || !S_ISREG(st.st_mode))
        errror("Opening not reg file", LINE, FILE);

    return output;
}

void *mmap_safe(fd_t file, int LINE, const char *FILE)
{
    struct stat st;
    fstat(file, &st);

    void *out = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, file, 0);

    if (out == (void *)(-1))
        errror("Mmaping", LINE, FILE);

    return out;
}

void dup2_safe(fd_t old_fd, fd_t new_fd, int LINE, const char *FILE)
{
    int out_check = dup2(old_fd, new_fd);
    if (out_check == -1)
        errror("DUP2", LINE, FILE);
}
void write_safe(fd_t fd, void *buf, size_t size, int LINE, const char *FILE)
{
    ssize_t out_check = write(fd, buf, size);
    if (out_check == -1)
        errror("WRITE", LINE, FILE);
}
void read_safe(fd_t fd, void *buf, size_t size, int LINE, const char *FILE)
{
    ssize_t out_check = read(fd, buf, size);
    if (out_check == -1)
        errror("READ", LINE, FILE);
}

void errror(char info[], size_t LINE, const char *FILE)
{
    printf("Mistake was found in %Ilu in file: %s", LINE, FILE);
    printf("\n\nProblem: %s\n", info);
    perror("Problem in perror");
    printf("\n");
    abort();
}

void warrning(char info[], size_t LINE, const char *FILE)
{
    printf("Warning was found in %Ilu in file: %s", LINE, FILE);
    printf("\n\nProblem: %s\n", info);
    perror("Problem in perror");
    printf("\n");
}

static sem_t get_shared_block(char *filename, int size, int small_key)
{
    key_t key = ftok(filename, small_key);
    if (key == MY_IPC_ERROR)
        return MY_IPC_ERROR;

    return shmget(key, size, 0666 | IPC_CREAT);
}

void *get_shared_memory(int size, int key, int LINE, const char *file)
{
    int shared_id = get_shared_block(FILE_SHARED_MEM, size, key);
    if (shared_id == MY_IPC_ERROR)
        errror("shmget or ftok wasn't done properly!", LINE, file);

    void *out_mem = shmat(shared_id, NULL, 0);
    if (out_mem == (void *)MY_IPC_ERROR)
        errror("Shmat wasn't done properly!", LINE, file);

    return out_mem;
}

int detach_shared_memory(void *block, int LINE, const char *file)
{
    int out = shmdt(block);
    if (out == MY_IPC_ERROR)
        errror("Shmdt wasn't done properly!", LINE, file);

    return out;
}

int destroy_shared_memory(int key, int LINE, const char *file)
{
    int shared_block_id = get_shared_block(FILE_SHARED_MEM, 0, key);

    if (shared_block_id == MY_IPC_ERROR)
        errror("Shmget wasn't done properly!", LINE, file);

    int out = shmctl(shared_block_id, IPC_RMID, NULL);
    if (out == MY_IPC_ERROR)
        errror("shctl wasn't done properly!", LINE, file);

    return out;
}

sem_t sem_get(int small_key)
{
    key_t key = ftok(FILE_SHARED_MEM, small_key);
    if (key == MY_IPC_ERROR)
    {
        ERROR("ftok wasn't done properly!");
    }

    int out = semget(key, 1, IPC_CREAT | 0666);
    if (out == MY_IPC_ERROR)
        ERROR("semget wasn't done properly!");

    return out;
}

static void initialize_semaphors_for_library(struct sembuf *sem, int op)
{
    sem->sem_flg = 0;
    sem->sem_num = 0;
    sem->sem_op = op;
}

void sem_increase(sem_t sem)
{
    static bool_t check = false;
    static struct sembuf buffer;

    if (!check)
    {
        check = true;
        initialize_semaphors_for_library(&buffer, 1);
    }

    if (semop(sem, &buffer, 1) == MY_IPC_ERROR)
        ERROR("Semaphor wasn't done properly!");
}
void sem_decrease(sem_t sem)
{
    static bool_t check = false;
    static struct sembuf buffer;

    if (!check)
    {
        check = true;
        initialize_semaphors_for_library(&buffer, -1);
    }

    if (semop(sem, &buffer, 1) == MY_IPC_ERROR)
        ERROR("Semaphor wasn't done properly!");
}
