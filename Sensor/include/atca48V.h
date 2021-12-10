/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created   2-DEC-2020   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/
#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

#ifdef __cplusplus
extern "C" {
#endif

struct atca48V {
  u16 status;
  float celsius;
  float voltageHoldUp;
  float voltageA;
  float voltageB;
  float current;
  u16 raw[6];
  u8 noUpd;
};

/* The generic init/read functions for each type of device on the I2C bus. */
void atca48VInit(struct sensorI2CAddress *sa);
void atca48VRead(struct sensorI2CAddress *sa, void *valueBuffer);
void atca48VFormat(struct sensorRecord *sr, struct cJSON *parent);

#ifdef __cplusplus
}
#endif

