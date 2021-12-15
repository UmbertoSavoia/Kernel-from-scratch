bits 32

section .multiboot          ; imposto i primi bits per il multiboot in GRUB
    dd 0x1BADB002           ; magic number per il bootloader
    dd 0x0                  ; flags a 0 per allineare i moduli a 4KB
    dd -(0x1BADB002 + 0x0)  ; checksum

section .text
global start
extern main

start:
    cli                     ; imposta a zero la flag di interruzione
    mov esp, stack_space    ; imposto il puntatore allo stack
    call main
    hlt                     ; blocca la CPU

section .bss
resb 8192                   ; riservo 8KB per lo stack
stack_space: