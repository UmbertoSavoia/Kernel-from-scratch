#include "../include/kernel.h"
#include "../include/libc.h"

void    print_stack(void)
{
    uint32 nbyte = 128;
    uint8   *ebp = 0;
    struct stackframe *stk;
    uint32 MaxFrames = 10;

    __asm__ volatile ("movl %%ebp, %0" : "=r" (ebp) ::);
    stk = (struct stackframe *)ebp;

    printf("Call trace:\n");
    for(uint32 frame = 0; stk && frame < MaxFrames; ++frame) {
        printf("[ 0x%8x ] \n", stk->eip);
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