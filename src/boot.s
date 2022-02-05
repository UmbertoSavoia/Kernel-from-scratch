MALIGN    equ 1 << 0
MEMINFO   equ 1 << 1
FLAGS     equ MALIGN | MEMINFO
MAGIC     equ 0x1BADB002
CHECKSUM  equ -(MAGIC + FLAGS)

section .multiboot          ; imposto i primi bits per il multiboot in GRUB
align 4
    dd MAGIC                ; magic number per il bootloader
    dd FLAGS                ; flags a 0 per allineare i moduli a 4KB
    dd CHECKSUM             ; checksum

section .bss
align 16
stack_bottom:
    resb 16384          ; riservo 16 MB per lo stack
stack_top:

section .text
    global _start:function (_start.end - _start)
    extern main

_start:
    mov esp, stack_top      ; imposto il puntatore allo stack
    xor ebp, ebp
    cli                     ; imposta a zero la flag di interruzione
    push ebx                ; EBX: Must contain the 32-bit physical
                            ;      address of the Multiboot information
                            ;      structure provided by the boot loader
    push eax                ; EAX: Must contain the magic value ‘0x2BADB002’
                            ;      the presence of this value indicates to the
                            ;      operating system that it was loaded by
                            ;      a Multiboot-compliant boot loader
    call main
    hlt                     ; blocca la CPU

.end: