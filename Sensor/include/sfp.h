#ifndef SENSOR_SFP_H_
#define SENSOR_SFP_H_
/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created  29-NOV-2020   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/
#include "sensorData.h" 
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

struct sfp {
  u8 rawData[512]; /* This could eventually be dropped. */
  float temp;
  float vcc;
  float txPower;
  float rxPower;
  u16 rawTemp,rawVCC,rawTx,rawRx;
  u8 noUpd;
};

void sfpInit(struct sensorI2CAddress *sa);
void sfpRead(struct sensorI2CAddress *sa, void *valueBuffer);
void sfpFormat(struct sensorRecord *sr, struct cJSON *parent);

#ifdef __cplusplus
}
#endif 


#endif

