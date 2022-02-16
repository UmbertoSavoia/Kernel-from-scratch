#ifndef GDT_H
#define GDT_H

#define GDTBASE 0x00000800
#define GDTSIZE 0xFF

#define KERNEL_CODE_SELECTOR    0x08
#define KERNEL_DATA_SELECTOR    0x10
#define KERNEL_STACK_SELECTOR   0x18
#define USER_CODE_SELECTOR      0x20
#define USER_DATA_SELECTOR      0x28
#define USER_STACK_SELECTOR     0x30
#define TSS_SELECTOR            0x38

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

struct s_tss
{
    uint32 link;
    uint32 esp0;
    uint32 ss0;
    uint32 esp1;
    uint32 esp2;
    uint32 ss2;
    uint32 sr3;
    uint32 eip;
    uint32 eflags;
    uint32 eax;
    uint32 ecx;
    uint32 edx;
    uint32 ebx;
    uint32 esp;
    uint32 ebp;
    uint32 esi;
    uint32 edi;
    uint32 es;
    uint32 cs;
    uint32 ss;
    uint32 ds;
    uint32 fs;
    uint32 gs;
    uint32 ldtr;
    uint32 iopb;
} __attribute__((packed));

extern struct gdtdesc     kgdt[GDTSIZE]; // GDT
extern struct s_tss       tss;           // TSS
extern struct gdtr        kgdtr;         // GDTR

void init_gdt_desc(uint32 base, uint32 limite, uint8 acces, uint8 other, struct gdtdesc *desc);
void init_gdt(void);
void tss_load(int tss);

#endif