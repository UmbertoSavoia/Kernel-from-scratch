#include "../include/kernel.h"
#include "../include/libc.h"

void    get_cursor_position(int *x, int *y)
{
    uint16 pos = 0;

    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16)inb(0x3D5)) << 8;

    *y = pos / 80;
    *x = pos % 80;
}

void    scroll()
{
    memcpy(vga_buffer, vga_buffer + 80, 25 * 80 * sizeof(uint32));
    for (int i = 0; i < 80; ++i)
        vga_buffer[24 * 80 + i] = (((BLACK << 4) | WHITE) << 8) | ' ';
}

void    move_cursor(int y, int x)
{
    uint16 pos = 0;

    if (y < 0) {
        cursor.y = y = 0;
    } else if (x > 79 && y > 24) {
        scroll();
        cursor.x = x = 0;
        cursor.y = y = 23;
    } else if (y > 24) {
        scroll();
        cursor.y = y = 24;
    }

    pos = y * 80 + x;
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
    memcpy(tmp_screen, screen_backup, 25 * 80 * sizeof(uint32));
    memcpy(screen_backup, vga_buffer, 25 * 80 * sizeof(uint32));
    memcpy(vga_buffer, tmp_screen, 25 * 80 * sizeof(uint32));
    move_cursor(tmp_cursor.y, tmp_cursor.x);
    cursor.x = tmp_cursor.x;
    cursor.y = tmp_cursor.y;
}