/*
 * sensorSRTM.h
 *
 *  Created on: 
 *      Author: 
 */

#ifndef SENSORINITSRTM_HW1_H_
#define SENSORINITSRTM_HW1_H_

#include "xtypes.h"
#include "sensorData.h"
#include "sensorI2C.h"
#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize the sensor structures */
int sensorInitSRTM_hw1();

/* Get the number of sensors */
int getNSensors_hw1();

/* Get the pointer to the sensor structures */
struct sensorRecord* getSensorRecord_hw1();

#ifdef __cplusplus
}
#endif 

#endif /* SENSORINITSRTM_HW1_H_ */
