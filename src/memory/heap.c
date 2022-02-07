#include "../../include/kernel.h"
#include "../../include/libc.h"

uint8 is_aligned(void *ptr)
{
    return ((uint32)ptr % HEAP_BLOCK_SIZE) == 0;
}

uint32 align_value(uint32 val)
{
    if (!(val % HEAP_BLOCK_SIZE))
        return val;
    return ((val - (val % HEAP_BLOCK_SIZE)) + HEAP_BLOCK_SIZE);
}

uint32 heap_get_index_from_addr(heap *heap, void *addr)
{
    return ((uint32)(addr - heap->start_addr)) / HEAP_BLOCK_SIZE;
}

int heap_init(heap *heap, heap_table *table, void *start, void *end)
{
    if (!is_aligned(start) || !is_aligned(end))
        return -1;
    bzero(heap, sizeof(struct s_heap));
    heap->table = table;
    heap->start_addr = start;
    if ((((uint32)(end - start)) / HEAP_BLOCK_SIZE) != table->size)
        return -1;
    memset(table->entries, HEAP_BLOCK_TABLE_FREE, sizeof(uint8) * table->size);
    return 0;
}

int heap_get_index_start_block(heap *heap, uint32 total_block)
{
    int     block_start = -1;
    uint32  block_continue = 0;

    for (uint32 i = 0; i < heap->table->size; ++i) {
        if ((heap->table->entries[i] & 0xf) != HEAP_BLOCK_TABLE_FREE) {
            block_start = -1;
            block_continue = 0;
            continue;
        }
        if (block_start == -1)
            block_start = i;
        ++block_continue;
        if (block_continue == total_block)
            break;
    }
    return block_start == -1 ? -1 : block_start;
}

void *heap_malloc(heap *heap, uint32 size)
{
    uint32 total_block = align_value(size) / HEAP_BLOCK_SIZE;
    void *addr = 0;
    int start = 0;
    uint8 flag = 0;
    int end = 0;

    if ((start = heap_get_index_start_block(heap, total_block)) == -1)
        return 0;
    addr = heap->start_addr + (start * HEAP_BLOCK_SIZE);
    flag = HEAP_BLOCK_TABLE_TAKEN | HEAP_BLOCK_IS_FIRST;
    if (total_block > 1)
        flag |= HEAP_BLOCK_HAS_NEXT;
    end = (start + total_block) - 1;

    for (int i = start; i <= end; ++i) {
        heap->table->entries[i] = flag;
        flag = HEAP_BLOCK_TABLE_TAKEN;
        if (i != end)
            flag |= HEAP_BLOCK_HAS_NEXT;
    }
    return addr;
}

void heap_free(heap *heap, void *ptr)
{
    uint32 start = ((uint32)(ptr - heap->start_addr)) / HEAP_BLOCK_SIZE;

    for (uint32 i = start; i < heap->table->size; ++i) {
        uint8 entry = heap->table->entries[i];
        heap->table->entries[i] = HEAP_BLOCK_TABLE_FREE;
        if (!(entry & HEAP_BLOCK_HAS_NEXT))
            break;
    }
}

uint32 heap_get_size_addr(heap *heap, void *addr)
{
    int i = heap_get_index_from_addr(heap, addr);
    int ret = 0;

    do {
        ret++;
        i++;
    } while ((heap->table->entries[i] & HEAP_BLOCK_HAS_NEXT)
        && !(heap->table->entries[i] & HEAP_BLOCK_IS_FIRST));

    return ret * HEAP_BLOCK_SIZE;
}