#include "../include/kernel.h"
#include "../include/libc.h"

void    print_stack(void)
{
    uint32 nbyte = 128;
    uint8   *ebp = 0;

    __asm__ volatile ("movl %%ebp, %0" : "=r" (ebp) ::);

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