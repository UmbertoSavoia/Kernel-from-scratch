extern isr_handler, do_syscalls, isr_schedule_int, isr_general_protection_fault_exception, isr_page_fault_exception

%macro  SAVE_REGS 0
    pushad
    push ds
    push es
    push fs
    push gs
    push ebx
    mov bx,0x10
    mov ds,bx
    pop ebx
%endmacro

%macro  RESTORE_REGS 0
    pop gs
    pop fs
    pop es
    pop ds
    popad
%endmacro

%macro  INTERRUPT 1
global _asm_irq_%1
_asm_irq_%1:
    SAVE_REGS
    push %1
    call isr_handler
    pop eax
    mov al,0x20
    out 0x20,al
    RESTORE_REGS
    iret
%endmacro

global _asm_syscalls, _asm_exception_general_protection_fault, _asm_exception_page_fault
_asm_syscalls:
    SAVE_REGS
    push eax      ; trasmissione del numero di chiamata
    call do_syscalls
    pop eax
    cli
    sti
    RESTORE_REGS
    iret

_asm_exception_general_protection_fault:
    SAVE_REGS
    call isr_general_protection_fault_exception
    RESTORE_REGS
    add esp,4
    iret

_asm_exception_page_fault:
    SAVE_REGS
    call isr_page_fault_exception
    RESTORE_REGS
    add esp,4
    iret

global _asm_schedule
_asm_schedule:
    SAVE_REGS
    call isr_schedule_int
    mov al,0x20
    out 0x20,al
    RESTORE_REGS
    iret

INTERRUPT 1
INTERRUPT 2

global enable_interrupts
enable_interrupts:
    sti
    ret

global disable_interrupts
disable_interrupts:
    cli
    ret