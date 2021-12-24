OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)                       /* punto di accesso del kernel */

SECTIONS
{
    . = 1M;                         /* serve almeno 1MB di spazio */

    .text BLOCK(4K) : ALIGN(4K)     /* sezione text */
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)   /* sezione di sola lettura */
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)     /* sezione data */
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)      /* sezione bss */
    {
        *(COMMON)
        *(.bss)
    }
}