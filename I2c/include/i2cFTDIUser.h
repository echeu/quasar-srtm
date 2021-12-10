/*
 * $Id: 
 *-
 *-   Purpose and Methods: Provide an interface to the FTDI USB hardware often 
 *-      used as an i2c interface. This requires the external libftdi to be
 *-      available.
 *-
 *-     THIS CODE IS NOT INTENDED TO BE USED ON THE ZYNQ. IT IS USED WHEN THE
 *-     I2C HEADERS ON THE SRTM HAPPEN TO BE ACCESSED FROM AN ANALOG DEVICES 
 *-     OR OTHER EXTERNAL USB ADAPTER WHEN MAKING BENCH TESTS.
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created   4-DEC-2020   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/
#ifdef __cplusplus
extern "C" {

#include "xtypes.h"
typedef  Xuint8 AddressType;

struct i2cBus* getFTDIBusDefn();

/* These are generic low-level interface routines to the FTDI i2c library */
int I2CFtdiOpenDevice(char *devname);
int I2CFtdiCloseDevice(int fd);
int I2CFtdiWrite(int fd, Xuint8 addr, int nbytes, unsigned char *bytes);
int I2CFtdiRead(int fd, Xuint8 addr, int nbytes, unsigned char *bytes);
int I2CFtdiMuxEnable(int fd, Xuint8 muxaddr, Xuint8 channelNumber);
int I2CFtdiRegEnable(int fd, Xuint8 muxaddr, Xuint8 channelNumber);

#endif
#ifdef __cplusplus
}
#endif
