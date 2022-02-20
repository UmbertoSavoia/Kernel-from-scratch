#ifndef PROCESS_H
#define PROCESS_H

#include "kernel.h"
#include "task.h"
#include "signal.h"

typedef struct s_task t_task;
typedef void (*sighandler_t)(int);

#define MAX_PATH                4096
#define MAX_ALLOCATIONS_PROCESS 1024
#define MAX_CHILDREN_PROCESS    8
#define MAX_PROCESSES           100
#define MAX_QUEUE_SIGNALS       32

#define UID_USER 1000

typedef struct  s_heap_process
{
    void *ptr;
    uint32 size;
}               t_heap_process;

typedef struct  s_signal
{
    int signum;
    sighandler_t handler;
}               t_signal;

typedef enum {
    RUN,
    STOP,
    ZOMBIE,
    THREAD
}       e_status;

typedef struct  s_process
{
    uint32              pid;
    uint32              uid;
    char                process_name[MAX_PATH];
    e_status            status;
    t_task              *task;
    t_heap_process      heap_process[MAX_ALLOCATIONS_PROCESS];
    void                *memory_ptr;
    uint32              size_memory;
    void                *stack_ptr;
    void                *bss_ptr;
    void                *data_section_ptr;
    t_signal            *signals[MAX_QUEUE_SIGNALS];
    char                buffer[4096];
    uint32              entry;
    struct s_process    *father;
    struct s_process    *children[MAX_CHILDREN_PROCESS];
}               t_process;

extern t_process *processes[MAX_PROCESSES];
extern t_process *current_process;

int get_free_process_slot(void);
int load_process(uint32 *addr, uint32 size, t_process **process);
int terminate_process(t_process *process);

#endif