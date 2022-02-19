#include "../../include/kernel.h"
#include "../../include/libc.h"
#include "../../include/syscalls.h"
#include "../../include/process.h"
#include "../../include/api.h"

t_syscall syscall_cmd[MAX_SYSCALLS] = {
        0,
        &fork_syscall,
        &exit_syscall,
        &wait_syscall,
        &getuid_syscall,
        &signal_syscall,
        &kill_syscall,
        &write_syscall,
};

void *fork_syscall(interrupt_frame *frame)
{
    (void)frame;
    t_process *process = 0;
    int slot = 0;
    int slot_children = -1;
    for (int i = 0; i < MAX_CHILDREN_PROCESS; ++i) {
        if (current_process->children[i] != 0) {
            slot_children = i;
            break ;
        }
    }
    if (slot_children == -1)
        return (void *)-1;
    if ((slot = get_free_process_slot()) == -1)
        return (void *)-1;
    if (!(process = kcalloc(sizeof(t_process))))
        return (void *)-1;
    processes[slot] = process;
    current_process->children[slot_children] = process;
    if (!(process->memory_ptr = kcalloc(current_process->size_memory))) {
        kfree(process);
        return (void *)-1;
    }
    memcpy(process->memory_ptr, current_process->memory_ptr, current_process->size_memory);
    process->size_memory = current_process->size_memory;
    memcpy(process->process_name, current_process->process_name, sizeof(MAX_PATH));
    process->pid = current_process->pid;
    process->entry = current_process->entry;
    process->uid = current_process->uid;
    process->father = current_process;
    if (!(process->stack_ptr = kcalloc(PROCESS_STACK_SIZE))) {
        kfree_multi(2, process->memory_ptr, process);
        return (void *)-1;
    }
    if (!(process->task = new_task(process))) {
        kfree_multi(3, process->stack_ptr, process->memory_ptr, *process);
        return (void *)-1;
    }

    for (uint32 i = 0; i < current_process->size_memory; ++i) {
        ((uint32 *)process->memory_ptr)[i] = ((uint32 *)current_process->memory_ptr)[i];
    }
    for (uint32 i = 0; i < PROCESS_STACK_SIZE; ++i) {
        ((uint32 *)process->stack_ptr)[i] = ((uint32 *)current_process->stack_ptr)[i];
    }
    if (map_page_directory_to(process->task->page_directory,
                              (uint32 *)process->entry,
                              process->memory_ptr,
                              (uint32 *)align_value(((uint32)process->memory_ptr) + process->size_memory, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task(process->task);
        kfree_multi(3, process->stack_ptr, process->memory_ptr, process);
        return (void *)-1;
    }

    if (map_page_directory_to(process->task->page_directory,
                              (uint32 *)PROCESS_VIRTUAL_STACK_END,
                              (uint32 *)process->stack_ptr,
                              (uint32 *) align_value((uint32)process->stack_ptr + PROCESS_STACK_SIZE, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task(process->task);
        kfree_multi(3, process->stack_ptr, process->memory_ptr, process);
        return (void *)-1;
    }
    process->task->registers.edi = current_process->task->registers.edi;
    process->task->registers.esi = current_process->task->registers.esi;
    process->task->registers.ebp = current_process->task->registers.ebp;
    process->task->registers.ebx = current_process->task->registers.ebx;
    process->task->registers.edx = current_process->task->registers.edx;
    process->task->registers.ecx = current_process->task->registers.ecx;
    process->task->registers.eax = process->pid;
    process->task->registers.eip = current_process->task->registers.eip;
    process->task->registers.cs = current_process->task->registers.cs;
    process->task->registers.flags = current_process->task->registers.flags;
    process->task->registers.esp = current_process->task->registers.esp;
    process->task->registers.ss = current_process->task->registers.ss;
    return 0;
}

void *wait_syscall(interrupt_frame *frame)
{
    (void)frame;
    for (int i = 0; i < MAX_CHILDREN_PROCESS; ++i)
        if (current_process->children[i] != 0)
            current_process->status = STOP;
    return 0;
}

void *exit_syscall(interrupt_frame *frame)
{
    (void)frame;
    int ret = (int)get_arg_from_item_stack_task(task_current, 0);
    terminate_process(current_process);
    next_task();
    return (void *)ret;
}

// uid_t getuid(void);
void *getuid_syscall(interrupt_frame *frame)
{
    (void)frame;
    return (void *)current_process->uid;
}

// typedef void (*sighandler_t)(int);
// sighandler_t signal(int signum, sighandler_t handler);
void *signal_syscall(interrupt_frame *frame)
{
    (void)frame;
    int signum = (int) get_arg_from_item_stack_task(task_current, 0);
    sighandler_t handler = (sighandler_t) get_arg_from_item_stack_task(task_current, 1);
    int prev_sign = 0;

    if (signum < 1 || signum > 31)
        return SIG_ERR;
    prev_sign = push_back_queue_signal(current_process, signum, handler);
    return (void *)prev_sign;
}

// int kill(pid_t pid, int sig)
void *kill_syscall(interrupt_frame *frame)
{
    (void)frame;
    int pid = (int) get_arg_from_item_stack_task(task_current, 0);
    int sig = (int) get_arg_from_item_stack_task(task_current, 1);

    if ((pid < 0 && pid >= MAX_PROCESSES) || processes[pid] == 0)
        return (void *)-1;
    push_back_queue_signal(processes[pid], sig, 0);
    return 0;
}

// int write(char *str, int size)
void *write_syscall(interrupt_frame *frame)
{
    (void)frame;
    char *str = (char *) get_arg_from_item_stack_task(task_current, 0);
    int size = (int) get_arg_from_item_stack_task(task_current, 1);
    int ret = 0;

    char *tmp = kmalloc(size);
    if ((ret = copy_from_user(task_current, str, tmp, size)) == -1) {
        kfree(tmp);
        return (void *)-1;
    }
    printf("%s", tmp);
    kfree(tmp);
    return (void *)size;
}

void *syscalls_handler(int num, interrupt_frame *frame)
{
    if (num == 0 || num >= MAX_SYSCALLS)
        return 0;
    task_save_state(task_current, frame);
    return syscall_cmd[num](frame);
}