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
#include <string.h>

#include "sfp.h"
#include "cJSON.h"
#include "sensorI2C.h"

void sfpInit(struct sensorI2CAddress *sa) {
}

void sfpRead(struct sensorI2CAddress *sa, void *valueBuffer) {

  /* Where to store the results. */
  struct sfp *dest = (struct sfp*)(valueBuffer);
  if( !dest ) {
    dest->noUpd = 0;
    return;
  }

  /* Clear the structure data. No internal pointers allowed */
  memset(dest,0,sizeof(struct sfp));

  u8 reg = 0;
  /* Required manufacturer's ID section */
  u8 *data = sensorRead(sa,0x0,255); /* Can't read 256 bytes as a block. */
  if( !data ) {
    dest->noUpd = 1;
    return;
  }
  memcpy(dest->rawData,data,255);

  /* Check if extended data is present. It's OK if that's the case. */
  if( (dest->rawData[92] & 0x40) == 0 ) return;

  /* Extended info, including power */
  sa->deviceAddr += 1;
  data = sensorRead(sa,0x0,255);
  sa->deviceAddr -= 1;
  if( !data ) {
    dest->noUpd = 1;
    return;
  }
  memcpy(&(dest->rawData[256]),data,255);
  // hexdump(dest->rawData,512);
  dest->noUpd = 0;

  /* Unpack the extended data for a few specific fields. */
  int idx = 96 + 256;
  dest->rawTemp = (dest->rawData[idx++]<<8) | dest->rawData[idx++]; /* Offset 96, 97 in 2nd block */
  dest->rawVCC =  (dest->rawData[idx++]<<8) | dest->rawData[idx++]; /* Offset 98, 99 in 2nd block */
  idx += 2;
  dest->rawTx = (dest->rawData[idx++]<<8) | dest->rawData[idx++];   /* Offset 102, 103 in 2nd block */
  dest->rawRx = (dest->rawData[idx++]<<8) | dest->rawData[idx++];   /* Offset 104, 105 in 2nd block */

  /* and convert them to C, V, \mu W */
  dest->temp = ((float)(dest->rawTemp))/256.0;    /* Celsius */
  dest->vcc = ((float)(dest->rawVCC))/10000.0;    /* Volts; one LSB = 100 microV */
  dest->txPower = ((float)(dest->rawTx))/10.0;    /* microW; one LSB = 0.1 microW */
  dest->rxPower = ((float)(dest->rawRx))/10.0;    /* microW; one LSB = 0.1 microW */
  return;
}

void sfpFormat(struct sensorRecord *sfpSensor, struct cJSON *parent) {
  struct sfp *tmp = (struct sfp*)(sfpSensor->valueBuffer);
  if( !tmp ) return;

  cJSON *txrxarr = cJSON_CreateArray();
  cJSON_AddItemToArray(txrxarr,cJSON_CreateNumber(tmp->noUpd) );
  cJSON_AddItemToArray(txrxarr,cJSON_CreateString(&tmp->rawData[20])); /* Vendor */
  cJSON_AddItemToArray(txrxarr,cJSON_CreateString(&tmp->rawData[40])); /* PN */
  cJSON_AddItemToArray(txrxarr,cJSON_CreateNumber(tmp->txPower) );
  cJSON_AddItemToArray(txrxarr,cJSON_CreateNumber(tmp->rxPower) );
  cJSON_AddItemToObject(parent,"sfp",txrxarr);
}

