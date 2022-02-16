#ifndef TASK_H
#define TASK_H

#include "kernel.h"
#include "process.h"

#define PROCESS_STACK_SIZE          1024 * 16
#define PROCESS_VIRTUAL_STACK_START 0x3FF000
#define PROCESS_VIRTUAL_STACK_END   PROCESS_VIRTUAL_STACK_START - PROCESS_STACK_SIZE

typedef struct s_process t_process;

typedef struct s_registers
{
    uint32 edi;
    uint32 esi;
    uint32 ebp;
    uint32 ebx;
    uint32 edx;
    uint32 ecx;
    uint32 eax;
    uint32 eip;
    uint32 cs;
    uint32 flags;
    uint32 esp;
    uint32 ss;
}               t_registers;

typedef struct  s_task
{
    uint32          *page_directory;
    t_registers     registers;
    t_process       *process;
    struct s_task   *prev;
    struct s_task   *next;
}               t_task;

t_task *new_task(t_process *process);
void remove_from_list_task(t_task *task);
int init_task(t_task *task, t_process *process);
void switch_task(t_task *task);
void next_task(void);
void switch_task_page_directory(void);
void run_task(void);

// ASM
void enter_userland(t_registers *regs);
void restore_registers(t_registers *regs);
void set_user_data_segment(void);

#endif