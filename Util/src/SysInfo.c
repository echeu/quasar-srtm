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

#include "cJSON.h"
#include "json_provider.h"
#include "BoardInfo.h"
#include "SysInfo.h"

void sysinfoInit() {
  registerJSONProvider(sysinfoFormat);
}

int sysinfoFormat(cJSON *top) {
  if( !top ) return -1;
  cJSON_AddItemToObject(top,"hwid",cJSON_CreateNumber(getHWEFuse()));
  cJSON_AddItemToObject(top,"fdna",cJSON_CreateString(getFuseDNA()));
  cJSON *userregs = cJSON_CreateArray();
  const u32 *uregs = getUserRegisters();
  for( int i=0 ; i<8 ; i++ ) cJSON_AddItemToArray(userregs,cJSON_CreateNumber(uregs[i]));
  cJSON_AddItemToObject(top,"ureg",userregs);
  cJSON *jsonVers = cJSON_CreateObject();
  cJSON_AddItemToObject(top,"versions",jsonVers);
  cJSON_AddNumberToObject(jsonVers, "fwvers", 0);
  cJSON_AddNumberToObject(jsonVers, "swvers", 0);
  cJSON_AddItemToObject(top,"type",cJSON_CreateString(getBoardTypeName()));
  cJSON_AddItemToObject(top,"title",cJSON_CreateString("HL-LHC SRTM Zynq"));
  return 0;
}

