/*
 * sensorData.c
 *
 *  Created on: Aug 7, 2018
 *      Author: LArTest
 */

#define SENSORDATA_C
#include <string.h>
#include <stdio.h>
#include "sensorData.h"

/* These are the definitions of the sensors to be monitored! The sensorRecord
 * array is unique to a given board: LArC, SRTM, ... Board specific
 * initialization is in sensorInit() and it must call sensorRegister
 * to enable this code.
 */
static int unpackBufferOnly=0;
static int nsensors=0;
static struct sensorRecord *sensorList;

/* This is the mandatory registration routine for a given board sensor suite. */
int sensorRegister(int nsensorsOnBoard, struct sensorRecord *sensors, int unpack) {
  if( nsensors != 0 ) return 1; /* Error. Already called. */
  nsensors = nsensorsOnBoard;
  sensorList = sensors;
  unpackBufferOnly = unpack;
  printf("Registering %d sensors (buffer only = %d)\n",nsensors,unpackBufferOnly);
  return 0;
}

//int sensorInit();

/* This is the universal monitoring routine. */
static int init=0;
void sensorMonitor(int doPrint) {
  int is;
//  if( !init ) { sensorInit(); init=1; }

   /* Read the data directly over the I2C bus */
  if( !unpackBufferOnly ) {
    for( is=0 ; is<nsensors ; is++ ) {
      struct sensorI2CAddress *saI2C = &(sensorList[is].saI2C);
      void *sdI2C = sensorList[is].valueBuffer;
      if( saI2C->readfcn ) saI2C->readfcn(saI2C,sdI2C);
    }
  }

  /* Data will be stored in a local memory buffer by the IPMC. This is set  *
   *  up in the initialization block above, and then done automatically via *
   *  interrupts and the associated handlers. */
  else {
    for( is=0 ; is<nsensors ; is++ ) {
      struct sensorIPMCAddress *saIPMC = &(sensorList[is].saIPMC);
      u16 ipmcBufferOffset = saIPMC->ipmcBufferOffset;  /* Word, not byte offset */
      void *sdI2C = sensorList[is].valueBuffer;
      if( saIPMC->convertfcn ) saIPMC->convertfcn(ipmcBufferOffset,sdI2C);
    }
  }

  /* and now try a formatted dump */
  if( doPrint ) {
    cJSON *top = cJSON_CreateObject();
    sensorFormat(top);
    printf("%s\n",cJSON_Print(top));
    cJSON_Delete(top);
  }
}


/* Return a JSON formatted structure with all of the sensor data */
int sensorFormat(cJSON *parent) {
  cJSON *sj = cJSON_CreateObject();
  cJSON_AddItemToObject(parent,"sensor",sj);
  int is=0;
  for( is=0 ; is<nsensors ; is++ ) if( sensorList[is].saI2C.fmtfcn ) sensorList[is].saI2C.fmtfcn(&sensorList[is],sj);
  return 0;
}

/* And provide an array like access to the sensor data just in case */
int sensorCount() { return nsensors; }
const struct sensorRecord *sensorGet(int is) {
  if ( is<nsensors && is>=0 ) return &sensorList[is];
  return 0;
}

/* ------------------------------------------------------------------------- */
/* The rest of these are the sensor type specific functions for initializing,*/
/* reading and formatting the data.                                          */
/* ------------------------------------------------------------------------- */

static int sensorLastUpdate=0;
int sensorGetLastUpdate() { return sensorLastUpdate; }

