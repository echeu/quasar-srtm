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

/* Initialize the sensor structures */
int sensorInitSRTM_hw2();

/* Get the number of sensors */
int getNSensors_hw2();

/* Get the pointer to the sensor structures */
struct sensorRecord* getSensorRecord_hw2();

#ifdef __cplusplus
}
#endif 

#endif /* SENSORSRTM_H_ */
