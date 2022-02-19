extern isr_handler, syscalls_handler, isr_clock, isr_general_protection_fault_exception, isr_page_fault_exception, exception_handler

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

%macro  INTERRUPT_IRQ 1
global _asm_irq_%1
_asm_irq_%1:
    SAVE_REGS
    push dword %1
    call isr_handler
    pop eax
    mov al,0x20
    out 0x20,al
    RESTORE_REGS
    iret
%endmacro

%macro  INTERRUPT_EXCEPTION 1
global _asm_exception_%1
_asm_exception_%1:
    pushad
    push esp
    push dword %1
    call exception_handler
    add esp, 8
    mov al,0x20
    out 0x20,al
    popad
    iret
%endmacro

global _asm_syscalls
_asm_syscalls:
    cli
    pushad
    push esp    ; stack frame
    push eax    ; numero syscall
    call syscalls_handler
    mov dword[res], eax
    pop ebx
    pop ebx
    popad
    mov eax, [res]
    iretd

global _asm_schedule
_asm_schedule:
    SAVE_REGS
    call isr_clock
    mov al,0x20
    out 0x20,al
    RESTORE_REGS
    iret

global enable_interrupts
enable_interrupts:
    sti
    ret

global disable_interrupts
disable_interrupts:
    cli
    ret

global tss_load
tss_load:
    push ebp
    mov ebp, esp
    mov ax, [ebp+8] ; TSS Segment
    ltr ax
    pop ebp
    ret

INTERRUPT_IRQ 1
INTERRUPT_IRQ 2

INTERRUPT_EXCEPTION 0
INTERRUPT_EXCEPTION 1
INTERRUPT_EXCEPTION 2
INTERRUPT_EXCEPTION 3
INTERRUPT_EXCEPTION 4
INTERRUPT_EXCEPTION 5
INTERRUPT_EXCEPTION 6
INTERRUPT_EXCEPTION 7
INTERRUPT_EXCEPTION 8
INTERRUPT_EXCEPTION 9
INTERRUPT_EXCEPTION 10
INTERRUPT_EXCEPTION 11
INTERRUPT_EXCEPTION 12
INTERRUPT_EXCEPTION 13
INTERRUPT_EXCEPTION 14
INTERRUPT_EXCEPTION 15
INTERRUPT_EXCEPTION 16
INTERRUPT_EXCEPTION 17
INTERRUPT_EXCEPTION 18
INTERRUPT_EXCEPTION 19

section .data
    res: dd 0