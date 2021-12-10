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
#ifndef SENSORI2C_H_
#define SENSORI2C_H_

#include "xtypes.h"
#include "sensorData.h"  /* Need to factorize better! */

#ifdef __cplusplus
extern "C" {
#endif

#define XST_SUCCESS 0
#define XST_FAILURE 1

/* Generic read & write functions with error reporting */
u8* sensorRead(struct sensorI2CAddress *sa, u8 reg, int nbytes);
u8* sensorWrite(struct sensorI2CAddress *sa, u8 *vals, int nbytes);
int sensorI2CStatus();

#ifdef __cplusplus
}
#endif 

#endif
