#ifndef SENSOR_ADN1066
#define SENSOR_ADN1066
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
#include "xtypes.h"
#include "cJSON.h"
#include "sensorData.h"

#ifdef __cplusplus
extern "C" {
#endif

struct adn1066 {
  /* Raw data */
  u8 mid;  /* Manufacturer's code. Should be 0x41 */
  u8 rev;  /* bit 4 should be 0. */
  u8 err;  /* Non-zero indicates i2c transaction error */

  u8 fstat[2];
  u8 ovstat[2];
  u8 uvstat[2];
  u8 limstat[2];
  u8 gpistat;

  u8 prevstext; u8 prevseqst;
  u8 bbstat[5];
  u8 bbaddr;
};

  /* Register offsets. See AN-698 */
#define MIDREG   0xF4
#define REVREG   0xF5

#define FSTAT1   0xE0
#define FSTAT2   0xE1
#define OVSTAT1  0xE2
#define OVSTAT2  0xE3
#define UVSTAT1  0xE4
#define UVSTAT2  0xE5
#define LIMSTAT1 0xE6
#define LIMSTAT2 0xE7
#define GPISTAT  0xE8

#define PREVSTEXT 0XEA
#define PREVSEQST 0xEB
#define BBSTAT1  0xEC
#define BBSTAT2  0xED
#define BBSTAT3  0xEE
#define BBSTAT4  0xEF
#define BBSTAT5  0xF0
#define BBADDR   0xF1   /* Add to 0xF900 (base) to get next fault rec addr. */


/* The generic init/read functions for each type of device on the I2C bus. */
void adn1066Init(struct sensorI2CAddress *sa);
void adn1066Read(struct sensorI2CAddress *sa, void *valueBuffer);
void adn1066Format(struct sensorRecord *sr, struct cJSON *parent);

#ifdef __cplusplus
}
#endif

#endif
