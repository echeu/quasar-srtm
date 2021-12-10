/*
 * $Id: 
 *-
 *-   Purpose and Methods:  Encapsulate the direct I2C reading of the LArC
 *-     sensors. This uses a pseudo device driver structure so that different
 *-     processors (Phase I microblaze vs. HL-LHC Zynq) can share the core
 *-     code. Each board type must register an array of struct i2cBus elements
 *-     via sensorI2CBusRegister below.
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-
*/
#ifndef I2CBUS_H_
#define I2CBUS_H_

#include "xtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XST_SUCCESS 0
#define XST_FAILURE 1


/* Different systems might have different versions of low level routines
 * needed by sensorRead and sensorWrite. These board specific routines
 * are encapsulated in the structure below, and the generic methods above
 * call these. The definitions must be provided by an initialization call 
 * to sensorI2CBusRegister below, typically from sensorInit for the 
 * specific board type. For an example, see sensorSRTM.
 */
typedef int (i2cBusOpen)(char *devname);
typedef int (i2cBusClose)(int fd);
typedef int (i2cBusWrite)(int fd, Xuint8 addr, int nbytes, unsigned char *bytes);
typedef int (i2cBusRead)(int fd, Xuint8 addr, int nbytes, unsigned char *bytes);
typedef int (i2cBusMuxEnable)(int fd, Xuint8 muxaddr, Xuint8 channelNumber);
typedef int (i2cBusRegEnable)(int fd, Xuint8 muxaddr, Xuint8 channelNumber);
typedef int (i2cBusLock)(int fd);    /* Until unlock is called, use restart */
typedef int (i2cBusUnlock)(int fd);  /* Release the bus. */

/* This structure provides a device driver like interface to a given bus.
 * Specific boards must call I2CBusRegister in their initialization
 * routines. */
struct i2cBus {
  i2cBusOpen *open;
  i2cBusClose *close;
  i2cBusRead *read;
  i2cBusWrite *write;
  i2cBusMuxEnable *muxen;
  i2cBusRegEnable *regen;
  i2cBusLock *lock;        /* Really, start a RD/WR operation with a restart */
  i2cBusUnlock *unlock;    /* Undo this */
  int fd;
  char *name;
};

void i2cBusRegister(int nbus, struct i2cBus *busList);
struct i2cBus* geti2cBusInstance(int ibus);
int geti2cBusCount();
int geti2cStatus();
void seti2cStatus();

void seti2cDebugLevel(int debug);
int  geti2cDebugLevel();
  
#define I2CMUX_CHANNELMASK(ichan) (((1<<(ichan-1))&0xFF))
#define I2CREG_CHANNELMASK(ichan) (~(((1<<(ichan))&0xFFFF)))
#define PAGESIZE            16   /* Correct for mux on DevKit */

#ifdef __cplusplus
}
#endif 

#endif
