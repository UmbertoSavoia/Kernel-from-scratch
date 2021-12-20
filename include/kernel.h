#ifndef KERNEL_H
#define KERNEL_H

/**
 * Typedef
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

/**
 * VGA
 */
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
extern uint8           cursor_y;
extern uint8           cursor_x;

void    clear_vga_buffer(void);
void    new_line(void);
void    putchar(char c);
uint32  putstr(char *s);
void    move_cursor(uint8 y, uint8 x);

/**
 * IO
 */
uint8   inb(uint16 port);
void    outb(uint16 port, uint8 data);

/**
 * PS/2
 */
uint8   wait_ps2_read(void);
uint8   wait_ps2_write(void);
uint8   send_command_to_ps2(uint8 port, uint8 cmd, uint8 if_data, uint8 data, uint8 await);

#endif