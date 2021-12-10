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
  cJSON *jsonVers = cJSON_CreateObject();
  cJSON_AddItemToObject(top,"versions",jsonVers);
  cJSON_AddNumberToObject(jsonVers, "fwvers", 0);
  cJSON_AddNumberToObject(jsonVers, "swvers", 0);
  cJSON_AddItemToObject(top,"type",cJSON_CreateString(getBoardTypeName()));
  cJSON_AddItemToObject(top,"title",cJSON_CreateString("HL-LHC SRTM Zynq"));
  return 0;
}

