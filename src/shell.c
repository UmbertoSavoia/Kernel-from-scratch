#include "../include/kernel.h"
#include "../include/libc.h"

static char    *get_name_symbol(uint32 eip)
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
            symtab = shdr->sh_addr;
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