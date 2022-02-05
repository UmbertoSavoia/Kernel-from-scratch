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

/**
 * Main
 */
void    shutdown(void);
void    reboot(void);

#endif