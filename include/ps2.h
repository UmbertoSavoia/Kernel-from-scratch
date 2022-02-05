#ifndef PS2_H
#define PS2_H

uint8   wait_ps2_read(void);
uint8   wait_ps2_write(void);
uint8   send_command_to_ps2(uint8 port, uint8 cmd, uint8 if_data, uint8 data, uint8 await);
uint8   init_controller_ps2(void);

#endif