#include "../include/kernel.h"
#include "../include/libc.h"

void    print_stack(void)
{
    uint8   *ebp = 0;

    __asm__ volatile ("movl %%ebp, %0" : "=r" (ebp) ::);

    for(int i = 0; i < 4; ++i) {
        printf("%d  ", i);
        for (int j = 0; j < 16; ++j, ++ebp) {
            printf("%x ", *ebp);
        }
        printf("\n");
    }
}