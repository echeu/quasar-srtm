#ifndef ZYNQPL_H
#define ZYNQPL__H
/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created  18-JUN-2023   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/
#ifdef __cplusplus
extern "C" {
#endif

#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

struct zynqPLDataRecord {
  int regOffs;
  char *name;
  float scale;
  float offset;
  int raw;
  float value;
  int err;
  int type;  /* voltage or temperature */
};

struct zynqPL {
  int nzpl;
  struct zynqPLDataRecord *zynqData;
  int *baseaddr;
};

void zynqPLInit(struct sensorI2CAddress *sa);
void zynqPLRead(struct sensorI2CAddress *sa, void *valueBuffer);
void zynqPLFormat(struct sensorRecord *sr, struct cJSON *parent);


#ifdef __cplusplus
}
#endif
#endif
