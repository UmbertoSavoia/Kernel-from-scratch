#include "../../include/kernel.h"
#include "../../include/memory.h"
#include "../../include/libc.h"

int copy_from_user(t_task *task, void *user_ptr, void *kernel_ptr, int size)
{
    if (size >= PAGING_PAGE_SIZE)
        return -1;

    char *tmp = 0;
    uint32 directory_index = 0, table_index = 0, entry = 0, old = 0;
    uint32 *table = 0;

    if (!(tmp = kcalloc(size)))
        return -1;
    get_indexes_paging(tmp, &directory_index, &table_index);
    entry = task->page_directory[directory_index];
    table = (uint32 *)(entry & 0xfffff000);
    old = table[table_index];
    map_page_directory_to(task->page_directory, tmp, tmp, tmp + PAGING_PAGE_SIZE,
                          PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    switch_paging(task->page_directory);
    memcpy(tmp, user_ptr, size);
    switch_paging(kpage_directory);
    map_page_directory_to(task->page_directory, tmp, tmp, tmp + PAGING_PAGE_SIZE,
                          PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    table[table_index] = old;
    memcpy(kernel_ptr, tmp, size);
    kfree(tmp);
    return 0;
}