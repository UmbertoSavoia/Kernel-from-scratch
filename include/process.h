#ifndef PROCESS_H
#define PROCESS_H

#include "kernel.h"
#include "task.h"

typedef struct s_task t_task;

#define MAX_PATH 4096
#define MAX_ALLOCATIONS_PROCESS 1024
#define MAX_CHILDREN_PROCESS 8
#define MAX_PROCESSES 100

typedef struct  s_heap_process
{
    void *ptr;
    uint32 size;
}               t_heap_process;

typedef struct  s_process
{
    uint32              pid;
    char                process_name[MAX_PATH];
    char                status[42];
    t_task              *task;
    t_heap_process      heap_process[MAX_ALLOCATIONS_PROCESS];
    void                *memory_ptr;
    uint32              size_memory;
    void                *stack_ptr;
    uint8               signals[32];
    uint32              entry;
    struct s_process    *father;
    struct s_process    *children[MAX_CHILDREN_PROCESS];
}               t_process;

extern t_process *processes[MAX_PROCESSES];

int load_process(uint32 *addr, uint32 size, t_process **process);

#endif