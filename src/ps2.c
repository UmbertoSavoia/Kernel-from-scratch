#include "../include/kernel.h"

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

/*
uint8   init_controller_ps2(void)
{

}*/
