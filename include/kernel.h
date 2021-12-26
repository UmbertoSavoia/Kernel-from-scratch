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

typedef struct s_cursor {
    int x;
    int y;
} t_cursor;

#define         VGA_ADDRESS 0xB8000;
#define         BUF_SIZE 2200
extern uint32          screen_backup[BUF_SIZE];
extern uint16          *vga_buffer;
extern int             vga_index;
static int             line_index;
extern uint8           back_color;
extern uint8           fore_color;
extern t_cursor        cursor;
extern t_cursor        cursor_backup;
extern uint32          tmp_screen[BUF_SIZE];
extern t_cursor        tmp_cursor;
extern char            buffer_shell[4096];

void    clear_vga_buffer(void);
void    new_line(void);
void    putchar(char c);
uint32  putstr(char *s);
void    move_cursor(int y, int x);
void    get_cursor_position(int *x, int *y);
void    switch_screen(void);

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
void    reboot(void);
void    print_header(void);

/**
 * Shell
 */
#define LEN_PROMPT 3

struct stackframe {
    struct stackframe* ebp;
    uint32 eip;
};

typedef struct  s_cmds {
    char *name;
    void (*f)(void);
}               t_cmds;

static char    *get_name_symbol(uint32 eip);
void            print_stack(void);
void            handler_cmds(void);

/**
 * Multiboot Grub
 * http://git.savannah.gnu.org/cgit/grub.git/tree/doc/multiboot.h?h=multiboot
 */

#define CHECK_FLAG(flags, bit)  ((flags) & (1 <<(bit)))
#define SHT_SYMTAB  2
#define SHT_STRTAB  3
#define SHN_UNDEF   0

typedef uint32  Elf32_Addr;
typedef uint32  Elf32_Off;
typedef uint16  Elf32_Half;
typedef short   Elf32_Sword;
typedef uint32  Elf32_Word;

typedef struct {
    uint32   sh_name;
    uint32   sh_type;
    uint32   sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    uint32   sh_size;
    uint32   sh_link;
    uint32   sh_info;
    uint32   sh_addralign;
    uint32   sh_entsize;
} Elf32_Shdr;

typedef struct elf32_sym{
    Elf32_Word	st_name;
    Elf32_Addr	st_value;
    Elf32_Word	st_size;
    uint8       st_info;
    uint8       st_other;
    Elf32_Half	st_shndx;
} Elf32_Sym;

/* The symbol table for a.out.  */
typedef struct  multiboot_aout_symbol_table
{
    uint32 tabsize;
    uint32 strsize;
    uint32 addr;
    uint32 reserved;
}               multiboot_aout_symbol_table_t;

/* The section header table for ELF.  */
typedef struct  multiboot_elf_section_header_table
{
    uint32 num;
    uint32 size;
    uint32 addr;
    uint32 shndx;
}               multiboot_elf_section_header_table_t;

typedef struct multiboot_info
{
    /* Multiboot info version number */
    uint32 flags;

    /* Available memory from BIOS */
    uint32 mem_lower;
    uint32 mem_upper;

    /* "root" partition */
    uint32 boot_device;

    /* Kernel command line */
    uint32 cmdline;

    /* Boot-Module list */
    uint32 mods_count;
    uint32 mods_addr;

    union
    {
        multiboot_aout_symbol_table_t aout_sym;
        multiboot_elf_section_header_table_t elf_sec;
    } u;

    /* Memory Mapping buffer */
    uint32 mmap_length;
    uint32 mmap_addr;

    /* Drive Info buffer */
    uint32 drives_length;
    uint32 drives_addr;

    /* ROM configuration table */
    uint32 config_table;

    /* Boot Loader Name */
    uint32 boot_loader_name;

    /* APM table */
    uint32 apm_table;

    /* Video */
    uint32 vbe_control_info;
    uint32 vbe_mode_info;
    uint16 vbe_mode;
    uint16 vbe_interface_seg;
    uint16 vbe_interface_off;
    uint16 vbe_interface_len;

    uint32 framebuffer_addr_low;
    uint32 framebuffer_addr_high;
    uint32 framebuffer_pitch;
    uint32 framebuffer_width;
    uint32 framebuffer_height;
    uint8 framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT	2
    uint8 framebuffer_type;
    union
    {
        struct
        {
            uint32 framebuffer_palette_addr;
            uint16 framebuffer_palette_num_colors;
        };
        struct
        {
            uint8 framebuffer_red_field_position;
            uint8 framebuffer_red_mask_size;
            uint8 framebuffer_green_field_position;
            uint8 framebuffer_green_mask_size;
            uint8 framebuffer_blue_field_position;
            uint8 framebuffer_blue_mask_size;
        };
    };
} multiboot_t;

extern multiboot_t *multiboot_info;

/**
 * System Map
 */
struct multiboot_module {
    uint32 mod_start;
    uint32 mod_end;
    uint32 string;
    uint32 reserved;
} __attribute__ (( packed, may_alias ));

#endif