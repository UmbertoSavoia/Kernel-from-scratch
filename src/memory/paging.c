#include "../../include/kernel.h"
#include "../../include/libc.h"

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

void switch_paging(uint32 *directory)
{
    load_page_directory(directory);
    current_page_directory = directory;
}