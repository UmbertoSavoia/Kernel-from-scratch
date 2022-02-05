#ifndef VGA_H
#define VGA_H

enum vga_color {
    BLACK,          //0
    BLUE,           //1
    GREEN,          //2
    CYAN,           //3
    RED,            //4
    MAGENTA,        //5
    BROWN,          //6
    GREY,           //7
    DARK_GREY,      //8
    BRIGHT_BLUE,    //9
    BRIGHT_GREEN,   //10
    BRIGHT_CYAN,    //11
    BRIGHT_RED,     //12
    BRIGHT_MAGENTA, //13
    YELLOW,         //14
    WHITE,          //15
};

typedef struct s_cursor {
    int x;
    int y;
} t_cursor;

#define         VGA_ADDRESS 0xB8000;
#define         BUF_SIZE 2200
extern uint32          screen_backup[BUF_SIZE];
extern uint16          *vga_buffer;
extern int             vga_index;
int             line_index;
extern uint8           back_color;
extern uint8           fore_color;
extern t_cursor        cursor;
extern t_cursor        cursor_backup;
extern uint32          tmp_screen[BUF_SIZE];
extern t_cursor        tmp_cursor;
extern char            buffer_shell[4096];

void    clear_vga_buffer(void);
void    new_line(void);
void    putchar(char c);
uint32  putstr(char *s);
void    move_cursor(int y, int x);
void    get_cursor_position(int *x, int *y);
void    switch_screen(void);

#endif