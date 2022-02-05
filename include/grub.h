#ifndef GRUB_H
#define GRUB_H

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

#endif