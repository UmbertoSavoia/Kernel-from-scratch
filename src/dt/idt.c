#include "../../include/kernel.h"
#include "../../include/libc.h"

void schedule() {}
void do_syscalls(int num) { (void)num; }

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
    /* Init IRD */
    for (uint32 i = 0; i < IDTSIZE; i++)
        init_idt_desc(0x08, (uint32)_asm_schedule, INTGATE, &kidt[i]);

    /* Numeri  0 -> 31 sono per le eccezioni */
    init_idt_desc(0x08, (uint32) _asm_exception_general_protection_fault, INTGATE, &kidt[13]);  // General Protection Fault
    init_idt_desc(0x08, (uint32) _asm_exception_page_fault, INTGATE, &kidt[14]);                // Page Fault

    init_idt_desc(0x08, (uint32) _asm_schedule, INTGATE, &kidt[32]);
    init_idt_desc(0x08, (uint32) _asm_irq_1, INTGATE, &kidt[33]);                               // Tastiera

    // Syscall
    init_idt_desc(0x08, (uint32) _asm_syscalls, TRAPGATE, &kidt[48]);
    init_idt_desc(0x08, (uint32) _asm_syscalls, TRAPGATE, &kidt[128]); //48

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