/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created   4-DEC-2020   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/

#include <stdlib.h>

#include "i2cBus.h"
#include "i2cFTDIUser.h"

int I2CFtdiOpenDevice(char *devname) { return 0; }
int I2CFtdiCloseDevice(int fd)  { return 0; }
int I2CFtdiWrite(int fd, Xuint8 addr, int nbytes, unsigned char *bytes)  { return 0; }
int I2CFtdiRead(int fd, Xuint8 addr, int nbytes, unsigned char *bytes)  { return 0; }
int I2CFtdiMuxEnable(int fd, Xuint8 muxaddr, Xuint8 channelNumber)  { return 0; }
int I2CFtdiRegEnable(int fd, Xuint8 muxaddr, Xuint8 channelNumber)  { return 0; }

struct i2cBus* getFTDIBusDefn() {

  struct i2cBus *bus = malloc(sizeof(struct i2cBus));
  if( !bus ) return 0;

  bus->open = I2CFtdiOpenDevice;
  bus->close = I2CFtdiCloseDevice;
  bus->read = I2CFtdiRead;
  bus->write = I2CFtdiWrite;
  bus->muxen = I2CFtdiMuxEnable;
  bus->regen = I2CFtdiRegEnable;
  bus->lock = 0;
  bus->unlock = 0;
  bus->fd = -1;
  bus->name = 0;
  return bus;

}

