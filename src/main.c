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

int     main(void)
{
    vga_buffer = (uint16 *)VGA_ADDRESS;
    clear_vga_buffer(&vga_buffer);

    putstr("42");
    fore_color = RED;
    new_line();
    putstr("KFS - Umberto Savoia");

    new_line();
    fore_color = WHITE;
}