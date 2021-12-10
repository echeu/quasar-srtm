/*
 * json_provider.c
 *
 *  Created on: Aug 10, 2016
 *      Author: CHayes (So, you know who to blame)
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#include "json_provider.h"

typedef struct {
	char *tag;
	JSONCommandPtr fcn;
} JSONCommand;

#define NPROVIDERS_MAX 20

static int np=0;
static JSONProviderPtr JSONProviders[NPROVIDERS_MAX];

static int nc=0;
static JSONCommand JSONCommands[NPROVIDERS_MAX];

/* The Register fcn simply registers JSON generating routines to be called by http_response.
 * A provider is called any time an http get request is made and always fills information,
 * so they should be short and quck. */
int registerJSONProvider(JSONProviderPtr xyzMakeJSONStruct){
	int ip=0;
	for( ip=0 ; ip<np ; ip++ ) if( xyzMakeJSONStruct == JSONProviders[ip] ) return ip;

	if( np == NPROVIDERS_MAX ) return -1;

	JSONProviders[np++] = xyzMakeJSONStruct;
	return np-1;
}


int unregisterJSONProvider(JSONProviderPtr xzyMakeJSONStruct) {
	return -1;
}

int getJSONProviderCount() { return np; }

int fillJSONStruct(int i, cJSON *parent) { return (*(JSONProviders[i]))(parent); }

/* The Register fcn simply registers JSON generating routines to be called by http_response.
 * A command routine is called only when "tag" is the first fiel system element in the
 * URL. These are used to generate a command/response pattern used e.g. by the monitoring and
 * the eyescan. */
int registerJSONCommand(const char *cmdtag, JSONCommandPtr jptr) {
	printf("In registerJSONCommand: '%s'\n",cmdtag);
	int ic=0;
	for( ic=0 ; ic<nc ; ic++ ) if( !strcmp(cmdtag,JSONCommands[ic].tag) ) return ic;
	if( nc == NPROVIDERS_MAX ) return -1;

	JSONCommands[nc].tag = malloc(strlen(cmdtag)+1);
	strcpy(JSONCommands[nc].tag,cmdtag);
	JSONCommands[nc++].fcn = jptr;
	return nc-1;
}

int unregisterJSONCommand(const char *cmdtag) {
	return -1;
}

int executeJSONCommand(const char *url, cJSON *parent) {
	int ic=0;
	int notProcessed=1;
	for( ic=0 ; ic<nc && notProcessed ; ic++) {
		const char *tag = JSONCommands[ic].tag;
		if( !strncmp(url,tag,strlen(tag)) ) {
			notProcessed = 0;
			JSONCommands[ic].fcn(url,parent);
		}
	}
	return notProcessed;
}

void jp_cJSON_error(cJSON *parent, char *errstr) {
  if( !parent ) return;
  cJSON_AddItemToObject(parent,"error",cJSON_CreateString(errstr));
}

void jp_cJSON_warning(cJSON *parent, char *warnstr) {
  if( !parent ) return;
  cJSON_AddItemToObject(parent,"warning",cJSON_CreateString(warnstr));
}

void jp_cJSON_ok(cJSON *parent, char *okstr) {
  if( !parent ) return;
  if( !okstr || !strlen(okstr) ) okstr = "slick!";
  cJSON_AddItemToObject(parent,"ok",cJSON_CreateString(okstr));
}

