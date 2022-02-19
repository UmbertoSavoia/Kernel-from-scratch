#include "../../include/kernel.h"
#include "../../include/libc.h"

heap kheap = {0};
heap_table kheap_table = {0};

uint32 align_value(uint32 val, uint32 align_at)
{
    if (!(val % align_at))
        return val;
    return ((val - (val % align_at)) + align_at);
}

int     init_memory(void)
{
    kheap_table.entries = (uint8*)HEAP_TABLE_ADDRESS;
    kheap_table.size = HEAP_SIZE / HEAP_BLOCK_SIZE;

    if (heap_init(&kheap, &kheap_table, (void *)HEAP_ADDRESS, (void *)(HEAP_ADDRESS + HEAP_SIZE)) == -1)
        return -1;

    return 0;
}

void *kmalloc(uint32 size)
{
    return heap_malloc(&kheap, size);
}

void kfree(void *ptr)
{
    heap_free(&kheap, ptr);
}

void kfree_multi(int argc, ...)
{
    va_list ap;
    va_start(ap, argc);
    uint32 *ptr = 0;

    while (argc) {
        ptr = va_arg(ap, uint32*);
        kfree(ptr);
        argc--;
    }
}

void *kcalloc(uint32 size)
{
    void *ret = 0;

    if (!(ret = kmalloc(size)))
        return 0;
    bzero(ret, size);
    return ret;
}

uint32 get_size_addr(void *addr)
{
    return heap_get_size_addr(&kheap, addr);
}