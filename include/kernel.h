#ifndef KERNEL_H
#define KERNEL_H

/**
 * Typedef
 */
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

/**
 * GDT - Global Descriptor Table
 */

#define GDTBASE 0x00000800
#define GDTSIZE 0xFF

struct gdtr {
    uint16 limite; // Dimensione in byte totale della GDT - 1
    uint32 base; // Indirizzo fisico della GDT
} __attribute__ ((packed));

/*
 * Base        : Il campo base (32 bit) è formato dai campi BaseLow (16 bit), BaseMiddle (8 bit) e BaseHigh (8 bit).
 *               Questo campo indica l'indirizzo fisico da cui comincia il segmento.
 * Limit       : Il campo limit (20 bit) è formato dal campo LimitLow (16 bit), e i 4 bit più bassi del campo Granularity.
 *               Questo campo indica la lunghezza del segmento, non l'indirizzo finale!
 *               I 20 bit del campo limit sono da interpretare come numero di unità da 8 byte o come numero di pagina
 *               indirizzabili all'interno del segmento. Se dunque si utilizzano pagina da 4 KB (12 bit) e il campo limit
 *               ha il valore 0xFFFFF la dimensione del segmento è pari a 4 GB (dimensione massima di un segmento in una
 *               macchina a 32 bit). Quindi un segmento copre le aree di memoria comprese tra 'Base' e 'Base + Limit'.
 * Access      : Il campo access contiene diverse informazioni. Ogni bit specifica come viene utilizzato quel segmento.
 *               +-------+---------+------------------------------------------------------------------------------------------------------------------------------------------------+
 *               |  Bit  |  Nome   |                                                                  Descrizione                                                                   |
 *               +-------+---------+------------------------------------------------------------------------------------------------------------------------------------------------+
 *               | 7     | Present | Indica se il segmento è presente o no in memoria, viene impostato sempre ad 1                                                                  |
 *               | 6 - 5 | DPL     | Indica il livello di privilegio rispetto alla CPU di quel segmento                                                                             |
 *               | 4     | System  | Se impostato a 1, la CPU utilizza questo segmento per dati e codice, se impostato a 0, si tratta di un TSS (Task State Segment) o un Call Gate |
 *               | 3 - 0 | Type    | Indica al processore i diritti che ha sul segmento (read-only/write ad esempio)                                                                |
 *               +-------+---------+------------------------------------------------------------------------------------------------------------------------------------------------+
 * Granularity : Il campo granularity, così come il campo access fornisce diverse informazioni:
 *               +-------+----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 *               |  Bit  |      Nome      |                                                                                        Descrizione                                                                                         |
 *               +-------+----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 *               | 7     | Granularity    | se impostato ad 1, il campo limit viene moltiplicato per 4096. In questo modo si può ottenere come dimensione massima del segmento 4 GB, altrimenti, a 0, la dimensione massima è di 1 MB. |
 *               | 6     | Operation Size | Indica se il segmento viene utilizzato a 16 o a 32 bit                                                                                                                                     |
 *               | 5     | Reserved       | Deve essere impostato a 0                                                                                                                                                                  |
 *               | 4     | AVL            | Questo campo veniva utilizzato nei processori precedenti alla serie Pentium. Ora è utilizzabile liberamente dal sistema operativo                                                          |
 *               | 3 - 0 | LimitHigh      | I 4 bit più alti del campo Limit                                                                                                                                                           |
 *               +-------+----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 *
 */
struct gdtdesc {
    uint16 lim0_15;
    uint16 base0_15;
    uint8  base16_23;
    uint8  acces;
    uint8  lim16_19:4;
    uint8  other:4;
    uint8  base24_31;
} __attribute__ ((packed));

struct tss {
    uint16 previous_task, __previous_task_unused;
    uint32 esp0;
    uint16 ss0, __ss0_unused;
    uint32 esp1;
    uint16 ss1, __ss1_unused;
    uint32 esp2;
    uint16 ss2, __ss2_unused;
    uint32 cr3;
    uint32 eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint16 es, __es_unused;
    uint16 cs, __cs_unused;
    uint16 ss, __ss_unused;
    uint16 ds, __ds_unused;
    uint16 fs, __fs_unused;
    uint16 gs, __gs_unused;
    uint16 ldt_selector, __ldt_sel_unused;
    uint16 debug_flag, io_map;
} __attribute__ ((packed));

extern struct gdtdesc     kgdt[GDTSIZE]; // GDT
extern struct tss         default_tss;   // TSS
extern struct gdtr        kgdtr;         // GDTR

void init_gdt_desc(uint32 base, uint32 limite, uint8 acces, uint8 other, struct gdtdesc *desc);
void init_gdt(void);

/**
 * IDT - Interrupt Descriptor Table
 */

#define IDTBASE  0x00000000
#define IDTSIZE  0xFF
#define INTGATE  0x8E00     // usato per gestire gli interrupt
#define TRAPGATE 0xEF00     // usato per effettuare chiamate di sistema

struct idtr {
    uint16 limite;
    uint32 base;
} __attribute__ ((packed));

struct idtdesc {
    uint16 offset0_15;
    uint16 select;
    uint16 type;
    uint16 offset16_31;
} __attribute__ ((packed));

extern struct idtdesc   kidt[IDTSIZE];
extern struct idtr      kidtr;

void _asm_syscalls();
void _asm_exception_general_protection_fault(void);
void _asm_exception_page_fault(void);
void _asm_schedule();
void schedule();
void do_syscalls(int num);
void init_idt_desc(uint16 select, uint32 offset, uint16 type, struct idtdesc *desc);
void init_idt(void);
void init_pic(void);

/**
 * ISR - Interrupt Service Routine
 */

// IRQ - Interrupt Request
void _asm_irq_0();
void _asm_irq_1();

void isr_handler(int id);
void isr_schedule_int();
void isr_keyboard();
void isr_general_protection_fault_exception(void);
void isr_page_fault_exception(void);

/**
 * VGA
 */
enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

#define         VGA_ADDRESS 0xB8000;
extern const uint32    BUF_SIZE;
extern uint16          *vga_buffer;
extern uint32          vga_index;
static uint32          line_index;
extern uint8           back_color;
extern uint8           fore_color;
extern uint8           cursor_y;
extern uint8           cursor_x;

void    clear_vga_buffer(void);
void    new_line(void);
void    putchar(char c);
uint32  putstr(char *s);
void    move_cursor(uint8 y, uint8 x);

/**
 * IO
 */
uint8   inb(uint16 port);
void    outb(uint16 port, uint8 data);
void    outw(uint16 port, uint16 data);

/**
 * PS/2
 */
uint8   wait_ps2_read(void);
uint8   wait_ps2_write(void);
uint8   send_command_to_ps2(uint8 port, uint8 cmd, uint8 if_data, uint8 data, uint8 await);
uint8   init_controller_ps2(void);

/**
 * Main
 */
void    shutdown(void);

#endif