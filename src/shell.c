#include "../include/kernel.h"
#include "../include/libc.h"

void    print_header(void)
{
    printf("                   _   ________ _____     \n"
           "                  | | / /|  ___/  ___|    \n"
           "                  | |/ / | |_  \\ `--.    \n"
           "                  |    \\ |  _|  `--. \\  \n"
           "                  | |\\  \\| |   /\\__/ / \n"
           "                  \\_| \\_/\\_|   \\____/ \n\n"
           "*------------------------HELPER------------------------*\n"
           "|  *--------Shortcut------*  *---------Shell--------*  |\n"
           "|  |                      |  |                      |  |\n"
           "|  | ESC - Shutdown       |  | print-stack          |  |\n"
           "|  | F1  - Reboot         |  | help                 |  |\n"
           "|  | F2  - Switch screen  |  | shutdown             |  |\n"
           "|  | F3  - Print Stack    |  | reboot               |  |\n"
           "|  |                      |  |                      |  |\n"
           "|  *----------------------*  *----------------------*  |\n");
}

char    *get_name_symbol(uint32 eip)
{
    multiboot_elf_section_header_table_t *elf = 0;
    Elf32_Shdr  *shdr = 0;
    Elf32_Sym   *symtab = 0, *candidate = 0;
    char        *strtable = 0;
    uint32      symnum = 0, symvalue = 0;

    if (!CHECK_FLAG(multiboot_info->flags, 5)
        || !(elf = &(multiboot_info->u.elf_sec))
        || !(elf->num) || !(elf->shndx))
        return 0;
    for (uint32 i = 0; i < elf->num; ++i) {
        shdr = (Elf32_Shdr *)(elf->addr + (elf->size * i));
        if (shdr->sh_type == SHT_SYMTAB) {
            symtab = (Elf32_Sym *)shdr->sh_addr;
            symnum = shdr->sh_size / sizeof(Elf32_Sym);
        }
        else if (i == elf->shndx - 1)
            if (!(strtable = (char *)shdr->sh_addr))
                return 0;
    }
    for (uint32 i = 0; i < symnum; ++i) {
        if (symtab[i].st_value > symvalue && symtab[i].st_value <= eip) {
            candidate = &(symtab[i]);
            symvalue = symtab[i].st_value;
        }
    }
    return !candidate ? 0 : &(strtable[candidate->st_name]);
}

void    print_stack(void)
{
    uint32 nbyte = 128;
    uint8   *ebp = 0;
    struct stackframe *stk;
    uint32 MaxFrames = 10;

    stk = (struct stackframe *)__builtin_frame_address(0);
    ebp = (uint8 *)stk->ebp;

    printf("Call trace:\n");
    for(uint32 frame = 0; stk && frame < MaxFrames; ++frame) {
        printf("[ 0x%8x ] - %s\n", stk->eip, get_name_symbol(stk->eip));
        stk = stk->ebp;
    }

    printf("\nCode:\n");
    for (uint32 i = 0; i < nbyte; i+=16) {
        printf("%4x  ", i);
        for (uint32 j = i; j < ((i + 16) < nbyte ? (i + 16) : nbyte); ++j)
            printf("%2x ", ebp[j]);
        printf("    |");
        for (uint32 j = i; j < ((i + 16) < nbyte ? (i + 16) : nbyte); ++j)
            printf("%c", isprint(ebp[j]) ? ebp[j] : '.');
        printf("|\n");
    }
}

void    handler_cmds(void)
{
    int x = 0, y = 0, i = 0, size_cmds = 0;
    t_cmds cmd[] = {
            {.name = "print-stack", .f = &print_stack},
            {.name = "shutdown", .f = &shutdown},
            {.name = "reboot", .f = &reboot},
            {.name = "help", .f = &print_header}
    };

    get_cursor_position(&x, &y);
    bzero(buffer_shell, 4096);
    for (i = 0; i < (x - LEN_PROMPT); ++i)
        buffer_shell[i] = vga_buffer[y * 80 + (i + LEN_PROMPT)];
    buffer_shell[i] = 0;
    size_cmds = sizeof(cmd) / sizeof(*cmd);
    for (int z = 0; z < size_cmds; ++z) {
        if (!memcmp(buffer_shell, cmd[z].name, strlen(buffer_shell))) {
            printf("\n\n");
            cmd[z].f();
        }
    }
    printf("\n$> ");
}