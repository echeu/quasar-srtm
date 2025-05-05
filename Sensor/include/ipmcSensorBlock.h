/*
 * $Id: 
 *-
 *-   Purpose and Methods: Header file supporting decoding data from the IPMC
 *-    written to the SRTM over I2C. The data format is defined in
 *-
 *-    https://docs.google.com/spreadsheets/d/1f98kbn4wLA95h2TtSlzXKEspF87lHBsekBCV3-fatek/edit?pli=1&gid=951097202#gid=951097202
 *-
 *-   This could be streamlined by introducing a registry with the decoder and data value structs autodetected and created
 *-   by a call to the register. The registry keys would be boardid and i2cBlockVer below.
*/
#ifndef IPMCSENSORBLOCK_H_
#define  IPMCSENSORBLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

/* This header is common to all boards and all formats. The boardid and i2cBlockVer should be used to select decoders */
struct ipmcDataHeader {
  u8 seqid;
  u8 boardid;
  u8 ipmcVer;
  u8 ipmcRev;
  u8 i2cBlockVer;
  u8 status;
  u8 nBytes;
};

/* This is the base record used by all sensors. The void object is a board and version specific struct, e.g. SrtmTester_v1 below. */
struct ipmcDataRecord {
  struct ipmcDataHeader idh;
  u8 *rawSensorData;  /* Without the header. Data only. */
  u8 timestamp[8];
  void (*decoder)(struct ipmcDataRecord *idr, struct cJSON *parent );  /* Board type and i2c data format specific decoder function (pointer). */
  double *values; /* Board type and i2c data format specific value record. See, e.g., SrtmTester_v1 in the .c file */
};

void ipmcSensorBlockInit(struct sensorI2CAddress *sa);
void ipmcSensorBlockRead(struct sensorI2CAddress *sa, void *valueBuffer);
void ipmcSensorBlockFormat(struct sensorRecord *sr, struct cJSON *parent);
  
#ifdef __cplusplus
}
#endif
#endif
