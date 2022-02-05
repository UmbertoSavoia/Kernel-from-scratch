#ifndef ISR_H
#define ISR_H

// IRQ - Interrupt Request
void _asm_irq_0();
void _asm_irq_1();

void isr_handler(int id);
void isr_schedule_int();
void isr_keyboard();
void isr_general_protection_fault_exception(void);
void isr_page_fault_exception(void);

#endif