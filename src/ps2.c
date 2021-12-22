#include "../include/kernel.h"
#include "../include/libc.h"

uint8   wait_ps2_read(void)
{
    uint8 status = 0;

    do {
        status = inb(0x64);
    } while ((status & 0x1) == 0);
    return status;
}

uint8   wait_ps2_write(void)
{
    uint8 status = 0;

    do {
        status = inb(0x64);
    } while ((status & 0x2) != 0);
    return status;
}

/**
 * Invia comandi ad un controller PS/2
 * @param port porta IO
 * @param cmd comando da inviare
 * @param if_data se è presente un secondo byte
 * @param data secondo byte da inviare
 * @param await se è presente un byte di risposta
 * @return byte di risposta altrimenti 0
 */
uint8   send_command_to_ps2(uint8 port, uint8 cmd, uint8 if_data, uint8 data, uint8 await)
{
    uint8 ret = 0;

    wait_ps2_write();
    outb(port, cmd);

    if (if_data) {
        wait_ps2_write();
        outb(0x60, data);
    }
    if (await) {
        wait_ps2_read();
        ret = inb(0x60);
    }
    return ret;
}

/**
 *
 * @return 1 = error, 0 = ok
 */
uint8   init_controller_ps2(void)
{
    uint8 is_dual_channel = 0; // 0 = dual channel, 1 = single channel
    uint8 test1 = 0, test2 = 0;

    // Disattiva i dispositivi in entrambi i canali
    send_command_to_ps2(0x64, 0xad, 0, 0, 0);
    send_command_to_ps2(0x64, 0xa7, 0, 0, 0);

    // Svuota il buffer di output
    inb(0x60);

    // Leggo la configurazione del controller per conoscere il numero di canali
    is_dual_channel = send_command_to_ps2(0x64, 0x20, 0, 0, 1);
    is_dual_channel &= 0x20;
    if (!is_dual_channel)
        printf("#14PS/2#15 : Dual channel\n");
    else
        printf("#14PS/2#15 : Single channel\n");

    // Eseguo l'autotest del controller
    if (send_command_to_ps2(0x64, 0xaa, 0, 0, 1) == 0x55)
        printf("[ #2OK#15 ] - Test controller ps2\n");
    else
        printf("[ #4KO#15 ] - Test controller ps2\n");

    // Eseguo i test ai due canali
    test1 = send_command_to_ps2(0x64, 0xab, 0, 0, 1);
    if (!test1)
        printf("[ #2OK#15 ] - Test primo canale ps2\n");
    else
        printf("[ #4KO#15 ] - Test primo canale ps2\n");
    if (!is_dual_channel) {
        test2 = send_command_to_ps2(0x64, 0xa9, 0, 0, 1);
        if (!test2)
            printf("[ #2OK#15 ] - Test secondo canale ps2\n");
        else
            printf("[ #4KO#15 ] - Test secondo canale ps2\n");
    }
    if ((test1 + test2) > 0)
        return 1;

    // Abilito i dispositivi
    send_command_to_ps2(0x64, 0xae, 0, 0, 0);
    if (!is_dual_channel)
        send_command_to_ps2(0x64, 0xa8, 0, 0, 0);

    return 0;
}
