#ifndef SENSOR_DDR4_H_
#define SENSOR_DDR4_H_
/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
*/
#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ddr4 {
  char part[21];
};

/* The generic init/read functions for each type of device on the I2C bus. */
void ddr4Init(struct sensorI2CAddress *sa);
void ddr4Read(struct sensorI2CAddress *sa, void *valueBuffer);
void ddr4Format(struct sensorRecord *sr, struct cJSON *parent);


#ifdef __cplusplus
}
#endif

#endif
