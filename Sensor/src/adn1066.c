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

#include <string.h>
#include <stdio.h>

#include "adn1066.h"
#include "cJSON.h"
#include "json_provider.h"
#include "sensorI2C.h"

void adn1066Init(struct sensorI2CAddress *sa) {}

void adn1066Read(struct sensorI2CAddress *sa, void *valueBuffer) {
  struct adn1066 *astatus = (struct adn1066 *)valueBuffer;
  if( !astatus ) return;
  memset(astatus,0,sizeof(struct adn1066) );

  /* Do the reads in address blocks */
  int rsize = REVREG-REVREG+1;
  u8 *data = sensorRead(sa,MIDREG,rsize);
  if( !data ) { astatus->err = 1; return; }
  memcpy(astatus,data,rsize);

  rsize = GPISTAT-FSTAT1+1;
  data = sensorRead(sa,FSTAT1,rsize);
  if( !data ) { astatus->err = 2; return; }
  memcpy(&(astatus->fstat[0]),data,rsize);

  rsize = BBADDR-PREVSTEXT+1;
  data = sensorRead(sa,PREVSTEXT,rsize);
  if( !data ) { astatus->err = 3; return; }
  memcpy(&(astatus->prevstext),data,rsize);
}

void adn1066Format(struct sensorRecord *sr, struct cJSON *parent) {
  struct adn1066 *data = (struct adn1066*)(sr->valueBuffer);
  if( !data ) {
    jp_cJSON_error(parent,"adn1066 i2c read failed");
    return;
  }

  cJSON *aobj = cJSON_CreateObject();
  if( !aobj ) {
    jp_cJSON_error(parent,"adn1066 failed to allocate buffer");
    return;
  }

  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->mid));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->rev));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->err));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->fstat[0]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->fstat[1]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->ovstat[0]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->ovstat[1]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->uvstat[0]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->uvstat[1]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->limstat[0]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->limstat[1]));
  cJSON_AddItemToArray(aobj,cJSON_CreateNumber(data->gpistat));
  cJSON_AddItemToObject(parent,"adn1066",aobj);
  return;
}
