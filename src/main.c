#include "../include/kernel.h"
#include "../include/libc.h"

const uint32     BUF_SIZE = 2200;
uint16           *vga_buffer = 0;
uint32           vga_index = 0;
static uint32    line_index = 0;
uint8            back_color = BLACK;
uint8            fore_color = WHITE;
uint8            cursor_y = 0;
uint8            cursor_x = 0;
struct gdtdesc   kgdt[GDTSIZE]; // GDT
struct tss       default_tss;   // TSS
struct gdtr      kgdtr;         // GDTR
struct idtr      kidtr;         // IDTR
struct idtdesc   kidt[IDTSIZE]; // IDT table


void    shutdown(void)
{
    outw(0x604, 0x2000); // QEMU
    // outw(0x4004, 0x3400); // Virtualbox
}

int     main(void)
{
    vga_buffer = (uint16 *)VGA_ADDRESS;
    clear_vga_buffer();

    printf("%d\n", 42);
    printf("#4KFS - Umberto Savoia#15\n");

    init_gdt();

    if (init_controller_ps2() != 0)
        return 1;

    init_idt();
    init_pic();
    asm("sti");
}