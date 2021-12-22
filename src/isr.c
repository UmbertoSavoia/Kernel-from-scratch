#include "../include/kernel.h"
#include "../include/libc.h"

void isr_schedule_int() {}

void isr_keyboard()
{
    uint8 scan = inb(0x60);
    printf("Premuto %x ", scan);
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

// General Protection Fault
void isr_general_protection_fault_exception(void) {}
void isr_page_fault_exception(void) {}