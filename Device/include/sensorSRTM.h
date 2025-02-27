/*
 * sensorSRTM.h
 *
 *  Created on: 
 *      Author: 
 */

#ifndef SENSORSRTM_H_
#define SENSORSRTM_H_

#include "xtypes.h"
#include "sensorData.h"
#include "sensorI2C.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

int sensorInitSRTM_hw2();
void sensorMonitor(int doPrint);
int sensorFormat(cJSON *parent);

#ifdef __cplusplus
}
#endif 

#endif /* SENSORSRTM_H_ */
