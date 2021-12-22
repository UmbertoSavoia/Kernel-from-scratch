#include "../include/kernel.h"
#include "../include/libc.h"

void    get_cursor_position(uint8 *x, uint8 *y)
{
    uint16 pos = 0;

    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16)inb(0x3D5)) << 8;

    *y = pos / 80;
    *x = pos % 80;
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
        screen_backup[i] = (((BLACK << 4) | WHITE) << 8) | ' ';
    }
}

void    new_line(void)
{
    line_index++;
    vga_index = 0;
    move_cursor(++cursor.y, cursor.x = 0);
}

void    putchar(char c)
{
    uint8 color = 0;

    if (c == '\n') {
        new_line();
        return ;
    } else if (c == '\t') {
        for (uint8 i = 0; i < 8; ++i)
            putchar(' ');
        return ;
    }

    color = (back_color << 4) | fore_color;
    get_cursor_position(&vga_index, &line_index);
    vga_buffer[line_index * 80 + vga_index] = (color << 8) | c;
    vga_index++;
    move_cursor(cursor.y, ++cursor.x);
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

void    switch_screen(void)
{
    memset(tmp_screen, 0, BUF_SIZE);
    memset(&tmp_cursor, 0, sizeof(t_cursor));

    tmp_cursor.x = cursor_backup.x;
    tmp_cursor.y = cursor_backup.y;
    get_cursor_position(&cursor_backup.x, &cursor_backup.y);
    memcpy(tmp_screen, screen_backup, BUF_SIZE);
    memcpy(screen_backup, vga_buffer, BUF_SIZE);
    memcpy(vga_buffer, tmp_screen, BUF_SIZE);
    move_cursor(tmp_cursor.y, tmp_cursor.x);
    cursor.x = tmp_cursor.x;
    cursor.y = tmp_cursor.y;
}