/*
 * json_provider.h
  *-   Purpose and Methods: A centralized header file for the JSON_Provider method
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *  Created on: Aug 8, 2016
 *      Author: Christopher R. Hayes
 */

#ifndef JSON_PROVIDER_H_
#define JSON_PROVIDER_H_

// Includes to use cJSON
#include "cJSON.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*JSONProviderPtr)(cJSON *parent);
typedef int (*JSONCommandPtr)(const char *urlbase, cJSON *parent);

int registerJSONProvider(JSONProviderPtr jptr);
int unregisterJSONProvider(JSONProviderPtr jptr);
int getJSONProviderCount();
int fillJSONStruct(int i, cJSON *parent);

int registerJSONCommand(const char *cmdname, JSONCommandPtr jptr);
int unregisterJSONCommand(const char *cmdname);
int executeJSONCommand(const char *url, cJSON *parent);

/* Utilities to aid status messaging */
void jp_cJSON_error(cJSON *parent, char *errstr);
void jp_cJSON_warning(cJSON *parent, char *warnstr);
void jp_cJSON_ok(cJSON *parent, char *okstr);

#ifdef __cplusplus
}
#endif

#endif /* JSON_PROVIDER_H_ */
