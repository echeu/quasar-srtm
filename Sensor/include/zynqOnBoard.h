/*
 * $Id: 
 *-
 *-   Purpose and Methods: Header file supporting reading the Zynq internal
 *-    temperatures and voltages as a standard sensor.
 *-   Created  17-MAY-2021   John D. Hobbs
 *- $$
 *-
*/
#ifndef ZYNQONBOARD_H_
#define  ZYNQONBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

#define ZYNQ_INTERNAL_TEMPERATURE 0
#define ZYNQ_INTERNAL_VOLTAGE 1

struct zynqDataRecord {
  char *basename;
  char *easyname;
  float scale;
  float offset;
  int   raw;
  float value; 
  int err;
  int  type;      /* temperature (0) or voltage (1) */
};


struct zynqOnBoard {
  int nzs;
  struct zynqDataRecord *zynqData;
  double uptime; /* sec */
};

void zynqOnBoardInit(struct sensorI2CAddress *sa);
void zynqOnBoardRead(struct sensorI2CAddress *sa, void *valueBuffer);
void zynqOnBoardFormat(struct sensorRecord *sr, struct cJSON *parent);

#ifdef __cplusplus
}
#endif
#endif
