#include "../include/kernel.h"

void    clear_vga_buffer(uint16 **buffer)
{
    vga_index = 0;
    line_index = 0;
    for (uint32 i = 0; i < BUF_SIZE; ++i) {
        (*buffer)[i] = ' ';
    }
}

void    new_line(void)
{
    line_index++;
    vga_index = 0;
}

void    putchar(char c)
{
    uint8 color = 0;

    color = (back_color << 4) | fore_color;
    vga_buffer[line_index * 80 + vga_index] = (color << 8) | c;
    vga_index++;
    if (!(vga_index % 80))
        new_line();
}

void    putstr(char *s)
{
    while (*s) {
        putchar(*s);
        ++s;
    }
}