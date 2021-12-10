#ifndef SENSOR_PCA8575_H_
#define SENSOR_PCA8575_H_
/*
 * $Id: 
 *-
 *-   Purpose and Methods: Read and format a PCA8575 register
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

struct pca8575 {
  u16 data;
};

void pca8575Init(struct sensorI2CAddress *sa);
void pca8575Read(struct sensorI2CAddress *sa, void *valueBuffer);
void pca8575Format(struct sensorRecord *sr, struct cJSON *parent);

#ifdef __cplusplus
}
#endif

#endif
