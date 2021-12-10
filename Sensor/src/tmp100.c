/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-
*/

#include <stdio.h>

#include "tmp100.h"
#include "sensorI2C.h"

void tmp100Init(struct sensorI2CAddress *sa) {
  u8 buff[5];

  /*
  **	Configure the TMP100 -
  **	B0 = 0; Shut Down Bit - Continuous Conversion
  **	B1 = 0; Comparator Mode
  **	B2 = 0; Alert Pin Active Low
  ** B3, B4 = 0, 0; Consecutive Faults = 1
  ** B5, B6 = 0, 0; 9-Bits of Resolution
  ** B7 = 0; Alert - Not used
  */
  
  buff[0] = 0x01;		// Select Configuration Register
  buff[1] = 0x60;		// Init Configuration Register
  sensorWrite(sa,buff,2);
  
  buff[0] = 0x00;		// Select Temperature Register
  sensorWrite(sa,buff,1);
}

void tmp100Read(struct sensorI2CAddress *sa, void *valueBuffer) {
  /* Where to store the results. */
  struct tmp100 *dest = (struct tmp100*)(valueBuffer);
  
  u8 reg = 0;
  u8 *data = sensorRead(sa,reg,2);
  if( !data ) {
    dest->noUpd = 1;
    dest->raw = dest->celsius = 0;
    return;
  }

  /* Unpack two bytes forming the 12 bit value and store it. */
  dest->noUpd = 0;
  dest->raw = (data[0]<<4) | (data[1]>>4);
  dest->celsius = dest->raw/16.0;
  printf("Raw: 0x%04x, Converted: %f\n",dest->raw,dest->celsius);
}

void tmp100Format(struct sensorRecord *tmpSensor, struct cJSON *parent) {
  struct tmp100 *tmp = (struct tmp100*)(tmpSensor->valueBuffer);
  if( !tmp ) return;
  cJSON *tarr = cJSON_CreateArray();
  cJSON_AddItemToArray(tarr,cJSON_CreateString(tmpSensor->name));
  cJSON_AddItemToArray(tarr,cJSON_CreateNumber(tmp->noUpd) );  /* Does an individual item fail? */
  cJSON_AddItemToArray(tarr,cJSON_CreateNumber(tmp->celsius) );
  cJSON_AddItemToObject(parent,"tmp100",tarr);
}


