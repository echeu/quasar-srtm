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

#include "sensorI2C.h"
#include "pca8575.h"

void pca8575Init(struct sensorI2CAddress *sa) { }

void pca8575Read(struct sensorI2CAddress *sa, void *valueBuffer) {
  struct pca8575 *tmp = (struct pca8575*)valueBuffer;
  if( !tmp ) return;

  u8 *data = sensorRead(sa,0x0,2);
  if( !data ) { memset(tmp,0,sizeof(struct pca8575)); return; }
  tmp->data = data[1]<<8 | data[0];

}

void pca8575Format(struct sensorRecord *sr, struct cJSON *parent) {
  struct pca8575 *tmp = (struct pca8575*)(sr->valueBuffer);
  cJSON *reg = cJSON_CreateNumber(tmp->data);
  cJSON_AddItemToObject(parent,"pca8575",reg);
}

