#ifndef I2CEEPROM_H
#define I2CEEPROM_H

#include "xtypes.h"
#include "i2cDevUser.h"

#define MUX_ADDR            0x74
#define EEPROM_ADDR         0x54
#define MUX_CHANNEL_KINTEX7 0x01 /* Yes, even on the ZCU111... */

/* Routines to read and write the EEPROM on the ZCU111. Wrappers around I2CRead and I2CWrite */
int I2CEepromRead(int fd, Xuint8 address, int nbytes, Xuint8 *dataBuffer);
int I2CEepromWrite(int fd, Xuint8 addr, int nbytes, unsigned char *databytes);

#endif
