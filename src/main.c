#include "../include/kernel.h"
#include "../include/libc.h"

uint16          *vga_buffer = 0;
int             vga_index = 0;
uint32          screen_backup[BUF_SIZE] = {0};
static int      line_index = 0;
uint8           back_color = BLACK;
uint8           fore_color = WHITE;
t_cursor        cursor = {0};
t_cursor        cursor_backup = {0};
uint32          tmp_screen[BUF_SIZE] = {0};
t_cursor        tmp_cursor = {0};
struct gdtdesc  kgdt[GDTSIZE]; // GDT
struct tss      default_tss;   // TSS
struct gdtr     kgdtr;         // GDTR
struct idtr     kidtr;         // IDTR
struct idtdesc  kidt[IDTSIZE]; // IDT table


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