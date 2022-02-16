#include "../../include/kernel.h"
#include "../../include/task.h"

t_task *task_head = 0;
t_task *task_current = 0;

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
    return task;
}

void remove_from_list_task(t_task *task)
{
    free_page_directory(task->page_directory);
    if (task == task_current)
        task_current = task_current->next;
    if (task == task_head)
        task_head = task_head->next;
    task->prev->next = task->next;
    task->next->prev = task->prev;
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
    switch_paging(task->page_directory);
}

void next_task(void)
{
    t_task *next = task_current->next;
    switch_task(next);
    enter_userland(&next->registers);
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