#ifndef SENSOR_FIREFLY_H_
#define SENSOR_FIREFLY_H_
/*
 * $Id: 
 *-
 *-   Purpose and Methods: Define firefly monitoring data and routines to read and 
 *-    format the data for use by the sensor library.
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

#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

/* These are the module addresses for the different firefly types. */
#define FF25TXONLY 0x50
#define FF25RXONLY 0x54  /* ??? */
#define FF10TXRX   0x50  /* ??? */

/* These are the addresses of the I2C registers for the Firefly module pin-base control/status lines */
#define FFOutputReg 0x22 /* 2 bits per FF: selectL, resetL */
#define FFInputReg  0x21 /* 2 bits per FF: presentL, interruptL. Assumed to be FFOutputReg-1 */

/* These handle the control and status lines for a full Firefly module */
int fireflyIsPresent(struct sensorI2CAddress *ffsa);
int fireflyIsInInterrupt(struct sensorI2CAddress *ffsa);
int fireflyReset(struct sensorI2CAddress *ffsa);
int fireflyEnableI2C(struct sensorI2CAddress *ffsa);
int fireflyDisableI2C(struct sensorI2CAddress *ffsa);

/* ---------------------------------------------------------------------- */
/* This is the monitoring/status structure for a 4 channel Tx/Rx module */
struct firefly4 {
  u8 present;
  u8 status;
  u8 txDisable; /* This, and the following 2 are control regsiters! */
  u8 cdrEnable;
  u8 cdrRate;
  u8 los;
  u8 txfault;
  u8 cdrlol;
  u8 tempfault;
  u8 voltfault;
  u16 powerfault;
  u16 uptime;
  s8 temperatureRaw;
  u16 rxPowerRaw[4];
  float rxPower[4];
  char part[14];
  char serial[11];
  u8 id;
  u8 fwVers;
  u8 fwMinor;
  u8 fwRev;
};

/* and the associated sensor I/O routines */
void fireflyInit4(struct sensorI2CAddress *sa);
void fireflyRead4(struct sensorI2CAddress *sa, void *valueBuffer);
void fireflyFormat4(struct sensorRecord *sr, struct cJSON *parent);

/* ---------------------------------------------------------------------- */
/* This is the monitoring/status structure for a 12 channel Tx or Rx module */
struct firefly12 {
  u8 present;
  u8 status;
  u16 channelDisable; /* This, and the following are control regsiters! */
  u16 outputDisable;
  u16 cdrEnable;
  u16 cdrlol;
  u8 tempfault;
  u8 voltfault;
  u16 uptime;
  s8 temperatureRaw;
  char vendor[10];
  char part[15];
  char serial[10];
  u32 fwVers;
};

/* and the associated sensor I/O routines */
void fireflyInit12(struct sensorI2CAddress *sa);
void fireflyRead12(struct sensorI2CAddress *sa, void *valueBuffer);
void fireflyFormat12(struct sensorRecord *sr, struct cJSON *parent);
void getFFvals(double vals[], char **names);

#ifdef __cplusplus
}
#endif

#endif
