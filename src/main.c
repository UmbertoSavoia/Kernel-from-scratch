#include "../include/kernel.h"
#include "../include/libc.h"

const uint32 BUF_SIZE = 2200;
uint16 *vga_buffer = 0;
uint32 vga_index = 0;
static uint32 line_index = 0;
uint8 back_color = BLACK;
uint8 fore_color = WHITE;
uint8 cursor_y = 0;
uint8 cursor_x = 0;

void    shutdown(void)
{
    outw(0x604, 0x2000); // QEMU
    // outw(0x4004, 0x3400); // Virtualbox
}

int     main(void)
{
    vga_buffer = (uint16 *)VGA_ADDRESS;
    clear_vga_buffer();

    putstr("42");
    fore_color = RED;
    new_line();
    putstr("KFS - Umberto Savoia");

    new_line();
    fore_color = WHITE;

    if (init_controller_ps2() != 0)
        return 1;

    uint32 code = 0, tmp = 0;
    while(1) {
        wait_ps2_read();
        code = inb(0x60);
        if (code == 0xe0) {
            wait_ps2_read();
            while ((tmp = inb(0x60)) == 0xe0) {
                wait_ps2_read();
            }
            code = code << 8;
            code |= tmp;
        }
        printf("0x%x ", code);
    }
}