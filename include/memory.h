#ifndef MEMORY_H
#define MEMORY_H

#define HEAP_ADDRESS            0x01000000
#define HEAP_SIZE               104857600
#define HEAP_TABLE_ADDRESS      0x00007E00
#define HEAP_BLOCK_SIZE         4096
#define HEAP_BLOCK_TABLE_TAKEN  1
#define HEAP_BLOCK_TABLE_FREE   0
#define HEAP_BLOCK_HAS_NEXT     0b10000000
#define HEAP_BLOCK_IS_FIRST     0b01000000

struct multiboot_mmap_entry
{
    uint32 size;
    uint32 addr_low;
    uint32 addr_high;
    uint32 len_low;
    uint32 len_high;
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5
    uint32 type;
} __attribute__((packed));
typedef struct multiboot_mmap_entry multiboot_memory_map_t;

typedef struct  s_heap_table
{
    uint8       *entries;
    uint32      size;
}               heap_table;

typedef struct  s_heap
{
    heap_table  *table;
    void        *start_addr;
}               heap;

extern heap         kheap;
extern heap_table   kheap_table;

int  init_memory(void);
void *kmalloc(uint32 size);
void kfree(void *ptr);
void *kcalloc(uint32 size);
uint32 get_size_addr(void *addr);

/**
 * HEAP
 */
int heap_init(heap *heap, heap_table *table, void *start, void *end);
void *heap_malloc(heap *heap, uint32 size);
void heap_free(heap *heap, void *ptr);
uint32 heap_get_size_addr(heap *heap, void *addr);

/**
 * PAGING
 */

#define PAGING_CACHE_DISABLED           0b00010000
#define PAGING_WRITE_THROUGH            0b00001000
#define PAGING_ACCESS_FROM_ALL          0b00000100
#define PAGING_IS_WRITEABLE             0b00000010
#define PAGING_IS_PRESENT               0b00000001
#define PAGING_TOTAL_ENTRIES_PER_TABLE  1024
#define PAGING_PAGE_SIZE                4096

extern uint32 *current_page_directory;

void load_page_directory(uint32 *page_directory);
void enable_paging(void);

uint32 *new_page_directory(uint8 flags);
void switch_paging(uint32 *directory);

#endif