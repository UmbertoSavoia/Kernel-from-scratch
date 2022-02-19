#include "../../include/kernel.h"
#include "../../include/task.h"

t_task *task_head = 0;
t_task *task_current = 0;
uint32 count_task = 0;

t_task *new_task(t_process *process)
{
    t_task *task = 0;
    if (!(task = kcalloc(sizeof(t_task))))
        return 0;
    if ((init_task(task, process)) == -1) {
        kfree(task);
        return 0;
    }
    if (!task_head) {
        task_current = task_head = task;
        task_head->prev = task_head->next = task_head;
    } else {
        task->prev = task_head->prev;
        task->next = task_head;
        task_head->prev->next = task;
        task_head->prev = task;
    }
    count_task++;
    return task;
}

void remove_from_list_task(t_task *task)
{
    free_page_directory(task->page_directory);
    count_task--;
    if (!count_task) {
        task_head = 0;
        task_current = 0;
    } else {
        if (task == task_current)
            task_current = task_current->next;
        if (task == task_head)
            task_head = task_head->next;
        if (task->prev)
            task->prev->next = task->next;
        if (task->next)
            task->next->prev = task->prev;
    }
    kfree(task);
}

int init_task(t_task *task, t_process *process)
{
    if (!(task->page_directory = new_page_directory(PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL)))
        return -1;
    task->registers.eip = process->entry;
    task->registers.ss = USER_DATA_SELECTOR | 3;
    task->registers.cs = USER_CODE_SELECTOR | 3;
    task->registers.esp = PROCESS_VIRTUAL_STACK_START;
    task->process = process;
    return 0;
}

void switch_task(t_task *task)
{
    task_current = task;
    current_process = task->process;
    switch_paging(task->page_directory);
}

void next_task(void)
{
    t_task *next = 0;
    if (task_current) {
        next = task_current->next;
        if (next->process->status == STOP) {
            int c = 1;
            while (c) {
                c = 0;
                for (int i = 0; i < MAX_CHILDREN_PROCESS; ++i) {
                    if (next->process->children[i] != 0)
                        c = 1;
                }
                if (!c) {
                    next->process->status = RUN;
                } else {
                    next = next->next;
                }
            }
        }
        switch_task(next);
        exec_process_signal(current_process);
        enter_userland(&next->registers);
    }
    else {
        panic("Nessun processo da eseguire");
    }
}

void switch_task_page_directory(void)
{
    set_user_data_segment();
    switch_task(task_current);
}

void run_task(void)
{
    switch_task(task_head);
    enter_userland(&task_head->registers);
}

void set_current_page_task(void)
{
    set_user_data_segment();
    switch_task(task_current);
}

void *get_arg_from_item_stack_task(t_task *task, int index)
{
    void *res = 0;
    uint32 *ptr_stack = (uint32 *)task->registers.esp;

    switch_paging(task->page_directory);
    res = (void *)ptr_stack[index];
    switch_paging(kpage_directory);
    return res;
}

void task_save_state(t_task *task, interrupt_frame *frame)
{
    task->registers.eip = frame->eip;
    task->registers.cs = frame->cs;
    task->registers.flags = frame->flags;
    task->registers.esp = frame->esp;
    task->registers.ss = frame->ss;
    task->registers.eax = frame->eax;
    task->registers.ebp = frame->ebp;
    task->registers.ebx = frame->ebx;
    task->registers.ecx = frame->ecx;
    task->registers.edi = frame->edi;
    task->registers.edx = frame->edx;
    task->registers.esi = frame->esi;
}