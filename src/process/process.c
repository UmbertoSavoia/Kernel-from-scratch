#include "../../include/kernel.h"
#include "../../include/libc.h"
#include "../../include/process.h"

t_process *processes[MAX_PROCESSES] = {0};
t_process *current_process = 0;

int get_free_process_slot(void)
{
    int i = 0;

    for (i = 0; i < MAX_PROCESSES; ++i) {
        if (!processes[i])
            return i;
    }
    return -1;
}

int load_process(uint32 *addr, uint32 size, t_process **process)
{
    int slot = 0;

    if ((slot = get_free_process_slot()) == -1)
        return -1;
    if (!(*process = kcalloc(sizeof(t_process))))
        return -1;
    if (!((*process)->memory_ptr = kcalloc(size))) {
        kfree(*process);
        return -1;
    }
    memcpy((*process)->memory_ptr, addr, size);
    (*process)->size_memory = size;
    if (!((*process)->stack_ptr = kcalloc(PROCESS_STACK_SIZE))) {
        kfree((*process)->memory_ptr);
        kfree(*process);
        return -1;
    }
    memcpy((*process)->process_name, "function", sizeof(MAX_PATH));
    (*process)->pid = slot;
    (*process)->entry = 0x400000;
    if (!((*process)->task = new_task(*process))) {
        kfree((*process)->stack_ptr);
        kfree((*process)->memory_ptr);
        kfree(*process);
        return -1;
    }
    if (map_page_directory_to((*process)->task->page_directory,
                              (uint32 *)(*process)->entry,
                              (*process)->memory_ptr,
                              (uint32 *)align_value(((uint32)(*process)->memory_ptr) + (*process)->size_memory, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task((*process)->task);
        kfree((*process)->stack_ptr);
        kfree((*process)->memory_ptr);
        kfree(*process);
        return -1;
    }

    if (map_page_directory_to((*process)->task->page_directory,
                              (uint32 *)PROCESS_VIRTUAL_STACK_END,
                              (uint32 *)(*process)->stack_ptr,
                              (uint32 *) align_value((uint32)(*process)->stack_ptr + PROCESS_STACK_SIZE, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task((*process)->task);
        kfree((*process)->stack_ptr);
        kfree((*process)->memory_ptr);
        kfree(*process);
        return -1;
    }
    processes[slot] = *process;
    return 0;
}