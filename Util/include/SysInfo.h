#ifndef _SYSINFO_H_
#define _SYSINFO_H_
/*
 * $Id: 
 *-
 *-   Purpose and Methods: Make the first few records of the JSON web server return
 *-    data. See 000-JSONStructure.txt for the full record.
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "BoardInfo.h"

void sysinfoInit();
int sysinfoFormat(cJSON *top);

#ifdef __cplusplus
}
#endif
#endif
