#ifndef SYSCALLS_H
#define SYSCALLS_H

#define MAX_SYSCALLS 8

#include "kernel.h"

struct s_interrupt_frame
{
    uint32 edi;
    uint32 esi;
    uint32 ebp;
    uint32 reserved;
    uint32 ebx;
    uint32 edx;
    uint32 ecx;
    uint32 eax;
    uint32 eip;
    uint32 cs;
    uint32 flags;
    uint32 esp;
    uint32 ss;
} __attribute__((packed));
typedef struct s_interrupt_frame interrupt_frame;

void *fork_syscall(interrupt_frame *frame);
void *exit_syscall(interrupt_frame *frame);
void *wait_syscall(interrupt_frame *frame);
void *getuid_syscall(interrupt_frame *frame);
void *signal_syscall(interrupt_frame *frame);
void *kill_syscall(interrupt_frame *frame);
void *write_syscall(interrupt_frame *frame);
void *syscalls_handler(int num, interrupt_frame *frame);

typedef void* (*t_syscall)(interrupt_frame *frame);

#endif