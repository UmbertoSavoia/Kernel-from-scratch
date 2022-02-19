#include "../../include/kernel.h"

uint32 *current_page_directory = 0;

uint32 *new_page_directory(uint8 flags)
{
    uint32 *directory = 0;
    uint32 *page_table = 0;
    uint32 offset_page_table = 0;

    if (!(directory = kcalloc(PAGING_TOTAL_ENTRIES_PER_TABLE * sizeof(uint32))))
        return 0;
    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; ++i) {
        if (!(page_table = kcalloc(PAGING_TOTAL_ENTRIES_PER_TABLE * sizeof(uint32))))
            return 0;
        for (int j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; ++j)
            page_table[j] = (offset_page_table + (j * PAGING_PAGE_SIZE)) | flags;
        offset_page_table += PAGING_PAGE_SIZE * PAGING_TOTAL_ENTRIES_PER_TABLE;
        directory[i] = (uint32)page_table | flags | PAGING_IS_WRITEABLE;
    }
    return directory;
}

void free_page_directory(uint32 *page_directory)
{
    for (int i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; ++i)
        kfree((uint32 *)(((uint32)page_directory[i]) & 0xfffff000));
    kfree(page_directory);
}

void switch_paging(uint32 *directory)
{
    load_page_directory(directory);
    current_page_directory = directory;
}

int get_indexes_paging(void *vaddr, uint32 *index_directory, uint32 *index_table)
{
    if ((uint32)vaddr % PAGING_PAGE_SIZE)
        return -1;
    *index_directory = ((uint32)vaddr / (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE));
    *index_table = ((uint32)vaddr % (PAGING_TOTAL_ENTRIES_PER_TABLE * PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);
    return 0;
}

int map_page_directory_to(uint32 *page_directory, void *vaddr, void *paddr, void *paddr_end, int flags)
{
    uint32 directory_index = 0, table_index = 0, entry = 0;
    uint32 *table = 0;
    uint32 bytes = (uint32)paddr_end - (uint32)paddr;
    int count =  bytes / PAGING_PAGE_SIZE;

    if (((uint32)vaddr % PAGING_PAGE_SIZE) || ((uint32)paddr % PAGING_PAGE_SIZE)
    || ((uint32)paddr_end % PAGING_PAGE_SIZE) || ((uint32)paddr_end < (uint32)paddr))
        return -1;

    for (int i = 0; i < count;
        ++i, vaddr += PAGING_PAGE_SIZE, paddr += PAGING_PAGE_SIZE) {
        directory_index = 0;
        table_index = 0;
        get_indexes_paging(vaddr, &directory_index, &table_index);
        entry = page_directory[directory_index];
        table = (uint32 *)(entry & 0xfffff000);
        table[table_index] = (uint32)paddr | flags;
    }
    return 0;
}