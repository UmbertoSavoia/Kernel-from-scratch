#include "../include/kernel.h"
#include "../include/libc.h"

void isr_schedule_int() {}

void isr_keyboard()
{
    static char keycode[] = {
            0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
            '-', '=', 0, 0,
            'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
            0, 0,
            'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
            0, '\\',
            'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
            0, '*', 0, ' ', 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0,
            '7', '8', '9', '-',
            '4', '5', '6', '+',
            '1', '2', '3',
            '0', '.'
    };
    static uint8 shift;
    static uint8 ctrl;
    static uint8 altgr;
    static uint8 flag;
    static uint8 capslock;
    uint8 scancode = inb(0x60);

    if (scancode == 0xE0) {
        flag = 1;
    } else if (flag == 1) {
        flag = 0;
        switch (scancode) {
            case 0x1C:              // Enter keypad
                printf("\n");
                break;
            case 0x1D:
                ctrl = 1;
                break;
            case 0x38:
                altgr = 1;
                break;
            case 0x53:
                // canc(delete)
                break;
            case 0x9D:
                ctrl = 0;
                break;
            case 0xB8:
                altgr = 0;
                break;
            case 0x4B:
                if ((cursor.x - 1) < LEN_PROMPT)
                    return;
                move_cursor(cursor.y, --cursor.x);
                break;
            case 0x48:
                // Up arrow
                // move_cursor(--cursor.y, cursor.x);
                break;
            case 0x4D:
                move_cursor(cursor.y, ++cursor.x);
                break;
            case 0x50:
                // Down arrow
                // move_cursor(++cursor.y, cursor.x);
                break;
            default:
                break;
        }
    } else {
        switch (scancode) {
            case 0x01:
                shutdown();
                break;
            case 0x0e:
                //= backspace press;
                move_cursor(cursor.y, --cursor.x);
                putchar(' ');
                move_cursor(cursor.y, --cursor.x);
                break;
            case 0x0f:
                //= tab press;
                putchar('\t');
                break;
            case 0x1c:
                //putchar('\n'); // Enter
                handler_cmds();
                break;
            case 0x1d:
                ctrl = 1;
                break;
            case 0x2a:
                shift = 1;
                break;
            case 0x36:
                shift = 1;
                break;
            case 0x38:
                //alt press
                break;
            case 0x3a:
                capslock = !capslock;
                break;
            case 0x3B:
                //= F1 pressed;
                reboot();
                break;
            case 0x3C:
                //= F2 pressed;
                switch_screen();
                break;
            case 0x3D:
                //= F3 pressed;
                print_stack();
                break;
            case 0x3E:
                //= F4 pressed;
                break;
            case 0x3F:
                //= F5 pressed;
                break;
            case 0x40:
                //= F6 pressed;
                break;
            case 0x41:
                //= F7 pressed;
                break;
            case 0x42:
                //= F8 pressed;
                break;
            case 0x43:
                //= F9 pressed;
                break;
            case 0x44:
                //= F10 pressed;
                break;
            case 0x57:
                //= F11 pressed;
                break;
            case 0x58:
                //= F12 pressed;
                break;
            case 0x9D:
                ctrl = 0;
                break;
            case 0xAA:
                shift = 0;
                break;
            case 0xB6:
                shift = 0;
                break;
            case 0xB8:
                //= left alt released
                break;
            default:
                if (scancode < 84) {
                    if (shift || capslock)
                        putchar(keycode[scancode] - 32);
                    else
                        putchar(keycode[scancode]);
                }
                break;
        }
    }
}

void isr_handler(int id)
{
    switch (id){
        case 1:
            isr_keyboard();
            break;
        default:
            return;

    }
    // Invio EOI - End of interrupt
    outb(0x20,0x20);
    outb(0xA0,0x20);
}

void isr_general_protection_fault_exception(void)
{
    printf("#4General Protection Fault#15\n");
}

void isr_page_fault_exception(void)
{
    printf("#4Page Fault#15\n");
}