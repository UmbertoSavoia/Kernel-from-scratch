[BITS 32]

global program
program:
    ;getuid
    ;mov eax, 4
    ;int 0x80

    ;exit
    ;mov ebx, 99
    ;push dword ebx
    ;mov eax, 2
    ;int 0x80

    ;signal
    ;push loop
    ;push 2
    ;mov eax, 5
    ;int 0x80

    ;write
    push 15
    push str
    mov eax, 7
    int 0x80
loop:
    jmp loop

section .data
    str:
        db "hello from user", 0