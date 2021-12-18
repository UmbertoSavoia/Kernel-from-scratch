#include "../include/kernel.h"

uint8   inb(uint16 port)
{
    uint8 ret = 0;

    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void    outb(uint16 port, uint8 data)
{
    asm volatile ("outb %0, %1"::"a"(data), "Nd"(port));
}

void    move_cursor(uint8 y, uint8 x)
{
    uint16 pos = y * 80 + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8) ((pos >> 8) & 0xFF));
}

void    clear_vga_buffer(void)
{
    vga_index = 0;
    line_index = 0;
    for (uint32 i = 0; i < BUF_SIZE; ++i) {
        vga_buffer[i] = (((BLACK << 4) | WHITE) << 8) | ' ';
    }
}

void    new_line(void)
{
    line_index++;
    vga_index = 0;
    move_cursor(++cursor_y, cursor_x = 0);
}

void    putchar(char c)
{
    uint8 color = 0;

    if (c == '\n') {
        new_line();
        return ;
    }

    color = (back_color << 4) | fore_color;
    vga_buffer[line_index * 80 + vga_index] = (color << 8) | c;
    vga_index++;
    move_cursor(cursor_y, ++cursor_x);
    if (!(vga_index % 80))
        new_line();
}

uint32  putstr(char *s)
{
    uint32 ret = 0;

    for (ret = 0; *s; ++s, ++ret) {
        putchar(*s);
    }
    return ret;
}

