#include "../../include/kernel.h"
#include "../../include/libc.h"

void schedule() {}

void do_syscalls(int num)
{
    printf("Syscall: %d\n", num);
}

void init_idt_desc(uint16 select, uint32 offset, uint16 type, struct idtdesc *desc)
{
    desc->offset0_15 = (offset & 0xffff);
    desc->select = select;
    desc->type = type;
    desc->offset16_31 = (offset & 0xffff0000) >> 16;
    return;
}

void init_idt(void)
{
    uint32 f[] = {
            (uint32)&_asm_exception_0, (uint32)&_asm_exception_1, (uint32)&_asm_exception_2, (uint32)&_asm_exception_3,
            (uint32)&_asm_exception_4, (uint32)&_asm_exception_5, (uint32)&_asm_exception_6, (uint32)&_asm_exception_7,
            (uint32)&_asm_exception_8, (uint32)&_asm_exception_9, (uint32)&_asm_exception_10, (uint32)&_asm_exception_11,
            (uint32)&_asm_exception_12, (uint32)&_asm_exception_13, (uint32)&_asm_exception_14, (uint32)&_asm_exception_15,
            (uint32)&_asm_exception_16, (uint32)&_asm_exception_17, (uint32)&_asm_exception_18, (uint32)&_asm_exception_19,
    };

    /* Init IRD */
    for (uint32 i = 0; i < IDTSIZE; ++i)
        init_idt_desc(0x08, (uint32)_asm_schedule, INTGATE, &kidt[i]);

    // Numeri  0 -> 31 sono per le eccezioni
    for (uint32 i = 0; i < 20; ++i)
        init_idt_desc(0x08, f[i], INTGATE, &kidt[i]);

    // Numeri 32 -> 47 per IRQ
    init_idt_desc(0x08, (uint32) _asm_schedule, INTGATE, &kidt[32]);
    init_idt_desc(0x08, (uint32) _asm_irq_1, INTGATE, &kidt[33]);     // Tastiera

    // Numero 48 -> 255 per Syscall
    for (int i = 48; i < IDTSIZE; ++i)
        init_idt_desc(0x08, (uint32) _asm_syscalls, TRAPGATE, &kidt[i]); //48


    kidtr.limite = IDTSIZE * 8;
    kidtr.base = IDTBASE;

    // Copio IDT nella memoria
    memcpy((char *) kidtr.base, (char *) kidt, kidtr.limite);

    // Carico IDTR nella CPU
    asm("lidtl (kidtr)");

    printf("[ #2OK#15 ] #14IDT#15 : Inizializzato\n");
}

void init_pic(void)
{
    /* Inizializzazione del ICW1 */
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    /* Inizializzazione del ICW2 */
    outb(0x21, 0x20);    /* Inizio vettore = 32 */
    outb(0xA1, 0x70);    /* Inizio vettore = 96 */

    /* Inizializzazione del ICW3 */
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    /* Inizializzazione del ICW4 */
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /* Maschera gli Interrupt */
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    printf("[ #2OK#15 ] #14PIC#15 : Inizializzato\n");
}