#ifndef SENSOR_LTC2945_H_
#define SENSOR_LTC2945_H_
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
#include <stdio.h>
#include "xtypes.h"
#include "sensorData.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ltc2945 {
  float voltage;
  float current;
  float temperature; /* 0 ==> N.C. */
  u16   rawVolts, rawAmps, rawADIN;
  u8 noUpd;
};

void ltc2945Init(struct sensorI2CAddress *sa);
void ltc2945Read(struct sensorI2CAddress *sa, void *valueBuffer);
void ltc2495Write(struct sensorI2CAddress *sa, void *valueBuffer);
void ltc2945Format(struct sensorRecord *sr, struct cJSON *parent);

/* Calibration constants */
const float* c_zynq_diode();
const float *c_ltm4628();
const float *c_ltm4630();

#ifdef __cplusplus
}
#endif

#endif
