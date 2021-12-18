#ifndef KERNEL_H
#define KERNEL_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

#define         VGA_ADDRESS 0xB8000;
extern const uint32    BUF_SIZE;
extern uint16          *vga_buffer;
extern uint32          vga_index;
static uint32          line_index;
extern uint8           back_color;
extern uint8           fore_color;

void    clear_vga_buffer(uint16 **buffer);
void    new_line(void);
void    putchar(char c);
uint32  putstr(char *s);

#endif