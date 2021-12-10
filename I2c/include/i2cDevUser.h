#ifndef I2CDEVUSER_H_
#define I2CDEVUSER_H_
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
#ifdef __cplusplus
extern "C" {
#endif


#include "xtypes.h"
typedef  Xuint8 AddressType;

/* These are generic low-level interface routines to the I2C driver */
int I2COpenDevice(char *devname);
int I2CCloseDevice(int fd);
int I2CWrite(int fd, Xuint8 addr, int nbytes, unsigned char *bytes);
int I2CRead(int fd, Xuint8 addr, int nbytes, unsigned char *bytes);
int I2CMuxEnable(int fd, Xuint8 muxaddr, Xuint8 channelNumber);
int I2CRegEnable(int fd, Xuint8 muxaddr, Xuint8 channelNumber);
int I2CEnableRestart(int fd);
int I2CDisableRestart(int fd);
  
#ifdef __cplusplus
}
#endif

#endif
