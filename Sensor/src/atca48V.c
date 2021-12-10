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
#include <stdio.h>

#include "atca48V.h"
#include "sensorI2C.h"

void atca48VInit(struct sensorI2CAddress *sa) {
}

void atca48VRead(struct sensorI2CAddress *sa, void *valueBuffer) {
}

void atca48VFormat(struct sensorRecord *brickSensor, struct cJSON *parent) {
  /* And the 48V brick */
  struct atca48V *tmp = (struct atca48V *)(brickSensor->valueBuffer);
  if( !tmp ) return;

  /* Crude type safety: I've got to be called by someone expecting me. */
  if( brickSensor->saI2C.fmtfcn != atca48VFormat ) return;

  cJSON *barr = cJSON_CreateArray();
  cJSON_AddItemToArray(barr,cJSON_CreateNumber(tmp->noUpd));
  cJSON_AddItemToArray(barr,cJSON_CreateNumber(tmp->status));
  cJSON_AddItemToArray(barr,cJSON_CreateNumber(tmp->celsius));
  cJSON_AddItemToArray(barr,cJSON_CreateNumber(tmp->voltageHoldUp));
  cJSON_AddItemToArray(barr,cJSON_CreateNumber(tmp->voltageA));
  cJSON_AddItemToArray(barr,cJSON_CreateNumber(tmp->voltageB));
  cJSON_AddItemToArray(barr,cJSON_CreateNumber(tmp->current));
  cJSON_AddItemToObject(parent,"brick",barr);
}
