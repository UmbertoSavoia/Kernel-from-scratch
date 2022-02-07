#ifndef KERNEL_H
#define KERNEL_H

/**
 * Typedef
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "vga.h"
#include "io.h"
#include "ps2.h"
#include "shell.h"
#include "grub.h"
#include "memory.h"

extern uint32 *kpage_directory;

/**
 * Main
 */
void    shutdown(void);
void    reboot(void);
void    panic(const char *msg);

void disable_interrupts();
void enable_interrupts();

#endif