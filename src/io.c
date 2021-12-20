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