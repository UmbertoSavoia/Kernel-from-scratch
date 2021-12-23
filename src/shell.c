#include "../include/kernel.h"
#include "../include/libc.h"

void    print_stack(void)
{
    uint32 *ebp = 0;

    __asm__ volatile ("movl %%ebp, %0" : "=r" (ebp) ::);

}