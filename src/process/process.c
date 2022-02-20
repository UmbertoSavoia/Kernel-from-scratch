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
        kfree_multi(2, (*process)->memory_ptr, *process);
        return -1;
    }
    memcpy((*process)->process_name, "function", sizeof(MAX_PATH));
    (*process)->pid = slot;
    (*process)->entry = 0x400000;
    (*process)->uid = UID_USER;
    if (!((*process)->task = new_task(*process))) {
        kfree_multi(3, (*process)->stack_ptr, (*process)->memory_ptr, *process);
        return -1;
    }
    if (map_page_directory_to((*process)->task->page_directory,
                              (void *)(*process)->entry,
                              (void *)(*process)->memory_ptr,
                              (void *)align_value(((uint32)(*process)->memory_ptr) + (*process)->size_memory, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task((*process)->task);
        kfree_multi(3, (*process)->stack_ptr, (*process)->memory_ptr, *process);
        return -1;
    }

    if (map_page_directory_to((*process)->task->page_directory,
                              (void *)PROCESS_VIRTUAL_STACK_END,
                              (void *)(*process)->stack_ptr,
                              (void *) align_value((uint32)(*process)->stack_ptr + PROCESS_STACK_SIZE, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task((*process)->task);
        kfree_multi(3, (*process)->stack_ptr, (*process)->memory_ptr, *process);
        return -1;
    }
    if (!((*process)->bss_ptr = kmalloc(PAGING_PAGE_SIZE))) {
        remove_from_list_task((*process)->task);
        kfree_multi(3, (*process)->stack_ptr, (*process)->memory_ptr, *process);
    }
    if (!((*process)->data_section_ptr = kmalloc(PAGING_PAGE_SIZE))) {
        remove_from_list_task((*process)->task);
        kfree_multi(4, (*process)->stack_ptr, (*process)->memory_ptr, (*process)->bss_ptr, *process);
    }
    uint32 vaddr_bss_start = align_value((*process)->entry + (*process)->size_memory, PAGING_PAGE_SIZE);
    uint32 vaddr_data_start = align_value(vaddr_bss_start + PAGING_PAGE_SIZE, PAGING_PAGE_SIZE);
    if (map_page_directory_to((*process)->task->page_directory,
                              (void *)vaddr_bss_start,
                              (void *)(*process)->bss_ptr,
                              (void *) align_value((uint32)(*process)->bss_ptr + PAGING_PAGE_SIZE, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task((*process)->task);
        kfree_multi(5, (*process)->stack_ptr, (*process)->memory_ptr, *process, (*process)->bss_ptr, (*process)->data_section_ptr);
        return -1;
    }
    if (map_page_directory_to((*process)->task->page_directory,
                              (void *)vaddr_data_start,
                              (void *)(*process)->data_section_ptr,
                              (void *) align_value((uint32)(*process)->data_section_ptr + PAGING_PAGE_SIZE, PAGING_PAGE_SIZE),
                              PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL | PAGING_IS_WRITEABLE) == -1) {
        remove_from_list_task((*process)->task);
        kfree_multi(5, (*process)->stack_ptr, (*process)->memory_ptr, *process, (*process)->bss_ptr, (*process)->data_section_ptr);
        return -1;
    }
    processes[slot] = *process;
    return 0;
}

int terminate_process(t_process *process)
{
    for (uint32 i = 0; i < MAX_ALLOCATIONS_PROCESS; ++i) {
        map_page_directory_to(process->task->page_directory,
                              process->heap_process[i].ptr,
                              process->heap_process[i].ptr,
                              (uint32 *)align_value((uint32)(process->heap_process[i].ptr+process->heap_process[i].size), PAGING_PAGE_SIZE),
                              0);
        process->heap_process[i].ptr = 0;
        process->heap_process[i].size = 0;
        kfree(process->heap_process[i].ptr);
    }
    kfree_multi(4, process->memory_ptr, process->stack_ptr, process->bss_ptr, process->data_section_ptr);
    remove_from_list_task(process->task);
    processes[process->pid] = 0;
    for (uint32 i = 0; i < MAX_PROCESSES; ++i) {
        for (uint32 j = 0; j < MAX_CHILDREN_PROCESS; ++j) {
            if (processes[i]->children[j] == process) {
                processes[i]->children[j] = 0;
            }
        }
    }
    return 0;
}