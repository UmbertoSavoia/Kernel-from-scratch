#ifndef IO_H
#define IO_H

uint8   inb(uint16 port);
void    outb(uint16 port, uint8 data);
void    outw(uint16 port, uint16 data);

#endif