#ifndef ISR_H
#define ISR_H

struct s_interrupt_frame
{
    uint32 edi;
    uint32 esi;
    uint32 ebp;
    uint32 reserved;
    uint32 ebx;
    uint32 edx;
    uint32 ecx;
    uint32 eax;
    uint32 ip;
    uint32 cs;
    uint32 flags;
    uint32 esp;
    uint32 ss;
} __attribute__((packed));
typedef struct s_interrupt_frame interrupt_frame;

// IRQ - Interrupt Request
void _asm_irq_0();
void _asm_irq_1();

void _asm_exception_0();
void _asm_exception_1();
void _asm_exception_2();
void _asm_exception_3();
void _asm_exception_4();
void _asm_exception_5();
void _asm_exception_6();
void _asm_exception_7();
void _asm_exception_8();
void _asm_exception_9();
void _asm_exception_10();
void _asm_exception_11();
void _asm_exception_12();
void _asm_exception_13();
void _asm_exception_14();
void _asm_exception_15();
void _asm_exception_16();
void _asm_exception_17();
void _asm_exception_18();
void _asm_exception_19();

void isr_handler(int id);
void isr_schedule_int();
void isr_keyboard();

void exception_handler(int id, interrupt_frame *frame);

#endif