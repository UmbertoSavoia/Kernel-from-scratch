#include "../../include/kernel.h"
#include "../../include/libc.h"

void init_gdt_desc(uint32 base, uint32 limite, uint8 acces, uint8 other, struct gdtdesc *desc)
{
    desc->lim0_15 = (limite & 0xffff);
    desc->base0_15 = (base & 0xffff);
    desc->base16_23 = (base & 0xff0000) >> 16;
    desc->acces = acces;
    desc->lim16_19 = (limite & 0xf0000) >> 16;
    desc->other = (other & 0xf);
    desc->base24_31 = (base & 0xff000000) >> 24;
    return;
}

void init_gdt(void)
{
    default_tss.debug_flag = 0x00;
    default_tss.io_map = 0x00;
    default_tss.esp0 = 0x1FFF0;
    default_tss.ss0 = 0x18;

    // Inizializzo i segmenti gdt
    init_gdt_desc(0x0, 0x0, 0x0, 0x0, &kgdt[0]);          // Segment null
    init_gdt_desc(0x0, 0xFFFFF, 0x9B, 0x0D, &kgdt[1]);    // kernel code
    init_gdt_desc(0x0, 0xFFFFF, 0x93, 0x0D, &kgdt[2]);    // kernel data
    init_gdt_desc(0x0, 0x0, 0x97, 0x0D, &kgdt[3]);        // kernel stack

    init_gdt_desc(0x0, 0xFFFFF, 0xFF, 0x0D, &kgdt[4]);    // user ucode
    init_gdt_desc(0x0, 0xFFFFF, 0xF3, 0x0D, &kgdt[5]);    // user udata
    init_gdt_desc(0x0, 0x0, 0xF7, 0x0D, &kgdt[6]);        // user ustack

    init_gdt_desc((uint32) & default_tss, 0x67, 0xE9, 0x00, &kgdt[7]);    // descrittore del tss

    // Inizializzo la struttura gdtr
    kgdtr.limite = GDTSIZE * 8;
    kgdtr.base = GDTBASE;

    // Copio il gdtr nella sua area di memoria
    memcpy((char *) kgdtr.base, (char *) kgdt, kgdtr.limite);

    /* Carico nella CPU il gdtr */
    asm("lgdtl (kgdtr)");

    /* Inizializzo i segmenti */
    asm("   movw $0x10, %ax     \n \
            movw %ax, %ds       \n \
            movw %ax, %es       \n \
            movw %ax, %fs       \n \
            movw %ax, %gs       \n \
            ljmp $0x08, $next   \n \
            next:               \n");
    printf("[ #2OK#15 ] #14GDT#15 : Inizializzato\n");
}