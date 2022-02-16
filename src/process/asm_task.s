global enter_userland
enter_userland:
    mov ebp, esp
    mov ebx, [ebp+4]
    push dword [ebx+44]
    push dword [ebx+40]
    pushf
    pop eax
    or eax, 0x200
    push eax
    push dword [ebx+32]
    push dword [ebx+28]
    mov ax, [ebx+44]
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push dword [ebp+4]
    call restore_registers
    add esp, 4
    iretd

global restore_registers
restore_registers:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    mov edi, [ebx]
    mov esi, [ebx+4]
    mov ebp, [ebx+8]
    mov edx, [ebx+16]
    mov ecx, [ebx+20]
    mov eax, [ebx+24]
    mov ebx, [ebx+12]
    add esp, 4
    ret

global set_user_data_segment
set_user_data_segment:
    mov ax, (8 * 5) | 3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret