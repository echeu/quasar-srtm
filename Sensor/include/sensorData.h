#ifndef SENSORDATA_H_
#define SENSORDATA_H_
/*
 * $Id: 
 *-
 *-   Purpose and Methods: Define structures for the data for each I2C sensor. 
 *-   Some of these are just the sensor information from a device with other
 *-   primary functionalit. For example, an SFP insert, a Firefly or a power
 *-   sequencer. This is the complete set of monitorable devices. Each board
 *-   as a specific source file (e.g. sensorI2C for the LArC) which defines 
 *-   and fills the generic sensordata structure. 
 *-
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created  16-AUG-2017   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/

#include "xtypes.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sensorI2CAddress;   /* Forward reference: Avoid compiler warnings in the following 2 lines */
struct sensorRecord;       /* ditto */

/* These functions are provided for each type of sensor on the bus */
typedef void (initFunction)(struct sensorI2CAddress* sa);
typedef void (readFunction)(struct sensorI2CAddress *sa, void *valuesBuffer);
typedef void (formatFunction)(struct sensorRecord *asensor, cJSON *outBuffer);
typedef void* (calParamsFunction)();
//typedef void (printFunction)(struct sensorRecord *asensor, File *fp);

/* 
   This encapsulates all information needed to reach a specific I2C device. The
    main purpose is to include mux or enable register config if needed. 
*/
struct sensorI2CAddress {
  u8 i2cBusNumber;          /* Board specific bus index. See sensorI2C.h */
  u8 switchChannelSelector; /* Which I2C switch or register is needed to enable path? I2C address. 0 ==> None needed. */
  u8 switchChannel;         /* Channel number in the switch or register. If MSB set, register, else switch. */
  u8 deviceAddr;            /* 7-bit I2C address of device */
  initFunction *initfcn;    /* Pointer to (optionaL) device initialization function */
  readFunction *readfcn;    /* Pointer to device read function. Likely calls sensorRead. */
  formatFunction *fmtfcn;   /* Pointer to device data format functions. Returns JSON */
  calParamsFunction *params;/* Pointer to an arbitrary array of device specific parameters */
};

/* In Phase-I, the sensor data can be written into an FPGA buffer by the IPMC
   instead of having the FPGA query the bus. The data is in a packed record of
   16-bit wors. The convert function(s) unpack the data into the actual
   structure. The "ipmcBufferOffset", below is really just the index into the
   data storage of the memory for a specific sensor.
*/
typedef void (convertFunction)(u16 ipmcBufferOffset, void *valuesBuffer);
struct sensorIPMCAddress {
  u8 ipmcBufferOffset;
  convertFunction *convertfcn;
  calParamsFunction *params;  /* Pointer to an arbitrary array of device specific parameters */
};


/* This is the structure used by generic code to access each sensor device. This
 * is the common interface to any sensor. */
struct sensorRecord {
  struct sensorI2CAddress saI2C;   /* For direct I2C access. */
  struct sensorIPMCAddress saIPMC; /* For internal sensor data buffer. See, e.g., SensorAccess.c */
  void *valueBuffer;               /* Address of the storage for the raw and */
  char *name;                      /* formatted data. And a text name */
  char *tag;                       /* optional tag to disambiguate. default is null and unused */
};


/* ----------------------------------------------------------------------------
 * Call these to register and fill the sensor data. sensorMonitor handles the 
 * IPMC detection and resulting differences. Each board type (LArC, SRTM, tester)
 * must provide a board specific sensorInit routine. This can cause linking 
 * conflicts if not carefully handled. The sensorInit routine must call 
 * sensorRegister once and only once with a point to the complete list
 * of sensors for the board.
 */
/* int sensorInit(); Implement this on a per board basis */
int sensorRegister(int nsensorsOnBoard, struct sensorRecord *sensors, int unpack);
void sensorMonitor(int doPrint);
int sensorFormat(cJSON *top);

/* Generically, these shouldn't be used, but here is an array like interface to
 * the sensor records.
 */
int sensorCount();
const struct sensorRecord *sensorGet(int is);

/* Return the last update time as seconds since boot */
int sensorGetLastUpdate();

#ifdef __cplusplus
}
#endif 

#endif
