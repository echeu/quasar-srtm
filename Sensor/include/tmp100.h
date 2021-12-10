#ifndef SENSOR_TMP100_H_
#define SENSOR_TMP100_H_
/*
 * $Id: 
 *-
 *-   Purpose and Methods: Define the sensor information and methods for a TMP100
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
*/
#include <stdio.h>
#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tmp100 {
  u16 raw;
  float celsius;
  u8 noUpd;
};

/* The generic init/read functions for each type of device on the I2C bus. */
void tmp100Init(struct sensorI2CAddress *sa);
void tmp100Read(struct sensorI2CAddress *sa, void *valueBuffer);
void tmp100Format(struct sensorRecord *sr, struct cJSON *parent);


#ifdef __cplusplus
}
#endif

#endif
