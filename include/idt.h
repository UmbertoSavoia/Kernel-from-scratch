#ifndef IDT_H
#define IDT_H

#define IDTBASE  0x00000000
#define IDTSIZE  0xFF
#define INTGATE  0x8E00     // usato per gestire gli interrupt
#define TRAPGATE 0xEF00     // usato per effettuare chiamate di sistema

struct idtr {
    uint16 limite;
    uint32 base;
} __attribute__ ((packed));

struct idtdesc {
    uint16 offset0_15;
    uint16 select;
    uint16 type;
    uint16 offset16_31;
} __attribute__ ((packed));

extern struct idtdesc   kidt[IDTSIZE];
extern struct idtr      kidtr;

void _asm_syscalls();
void _asm_exception_general_protection_fault(void);
void _asm_exception_page_fault(void);
void _asm_schedule();
void schedule();
void do_syscalls(int num);
void init_idt_desc(uint16 select, uint32 offset, uint16 type, struct idtdesc *desc);
void init_idt(void);
void init_pic(void);

#endif