#include "../include/kernel.h"
#include "../include/libc.h"

uint16          *vga_buffer = 0;
int             vga_index = 0;
uint32          screen_backup[BUF_SIZE] = {0};
int             line_index = 0;
uint8           back_color = BLACK;
uint8           fore_color = WHITE;
t_cursor        cursor = {0};
t_cursor        cursor_backup = {0};
uint32          tmp_screen[BUF_SIZE] = {0};
t_cursor        tmp_cursor = {0};
struct gdtdesc  kgdt[GDTSIZE] = {0}; // GDT
struct tss      default_tss;   // TSS
struct gdtr     kgdtr;         // GDTR
struct idtr     kidtr;         // IDTR
struct idtdesc  kidt[IDTSIZE] = {0}; // IDT table
multiboot_t     *multiboot_info = 0;
char            buffer_shell[4096] = {0};
uint32          *kpage_directory = 0;

void    reboot(void)
{
    uint8 good = 0x02;
    while (good & 0x02)
        good = inb(0x64);
    outb(0x64, 0xFE);
}

void    shutdown(void)
{
    outw(0x604, 0x2000); // QEMU
    // outw(0x4004, 0x3400); // Virtualbox
}

void    panic(const char *msg)
{
    disable_interrupts();
    printf("\n#4KERNEL PANIC#15 %s", msg);
    while (1);
}

int     main(uint32 magic, uint32 *info)
{
    if (magic != 0x2BADB002)
        return 1;
    multiboot_info = (multiboot_t *)info;
    vga_buffer = (uint16 *)VGA_ADDRESS;
    clear_vga_buffer();

    printf("%d - #4KFS - Umberto Savoia#15\n", 42);

    init_gdt();

    if (init_controller_ps2() != 0)
        return 1;

    init_idt();

    init_pic();

    if (init_memory() == -1) {
        printf("#4Memory error#15\n");
        return 1;
    } else {
        printf("[ #2OK#15 ] #14HEAP#15 : Inizializzato\n");
    }

    kpage_directory = new_page_directory(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    if (kpage_directory) {
        switch_paging(kpage_directory);
        enable_paging();
    } else {
        return 1;
    }

    //print_header();
    printf("\n$> ");

    enable_interrupts();

    /* Test syscall
        asm("int $0x30");
    */

    while(1);
    return 0;
}