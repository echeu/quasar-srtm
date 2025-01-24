/*
 * sensorSRTM.c
 *
 *  Created on: 
 *      Author: 
 *
 *   This is the master sensor data program for the HL-LHC SRTM Tester i2c sensor access.
 *   In addition to the ltc2945 and tmp100 sensors, it handles the status information
 *   from the SFP modules, the fireflies, the power good register, and the power 
 *   sequencer. It has a switch to also read out all sensors on the SRTM tester which
 *   are accessible on the SRTM i2c bus.
 *
 *   sensorInit calls both sensorI2CBusRegister and sensorRegister providing the 
 *   mandatory initialization sequence.
 *
 *   This code is based on the Phase I LArC sensor read out and retains, though does not
 *   at present use, the IPMC based filling of sensor data which was used in Phase I to
 *   avoid i2c bus conflicts.
 *
 */

#define SENSORSRTM_C

//#define ACCESS_SRTM_TESTER
#ifdef ACCESS_SRTM_TESTER

#include "malloc.h"
#include "string.h"

#include "xtypes.h"

#include "i2cBus.h"
#include "i2cDevUser.h"   /* Here "Dev" means /dev/i2c-X calls to access the bus. */

#include "tmp100.h"
#include "ltc2945.h"
#include "pca8575.h"
#include "sfp.h"

#include "sensorI2C.h"
#include "sensorData.h"
#include "sensorSRTM.h"

#include "cJSON.h"
#include "json_provider.h"

/* These are the i2C sensor counts. */
#define NTEMP 4      /* Temperatures from tmp100 */
#define NPOWER 11    /* VI ltc2945 */
#define NFFFELIX 2   /* 4 channel TX/RX modules */
#define NPSEQ 2


/*
 *  --- SRTM Tester sensors ----------------------------------------------------
 *  Start with the 4 temperature sensors
 */
#define TMP100A  0x48 /* U6 */
#define TMP100B  0x4A /* U7 */
#define TMP100C  0x4B /* U4 */
#define TMP100D  0x4C /* U5 */

/* Now we do the current/voltage sensors
*/
#define VI_P12V    0x67  /* Switch U64, chan 1 (starting at 1) */
#define VI_P12VRTM 0x68
#define VI_P3V3    0x6D
#define VI_P1V2    0x67  /* Switch U64, chan 2 (starting at 1) */
#define VI_P0V85   0x68
#define VI_P3V3a   0x69
#define VI_P2V5    0x6B
#define VI_P1V8    0x6C
#define VI_P2V5a   0x6D
#define VI_P0V9    0x6E
#define VI_P1V2a   0x6F

#define SFP 0x50

/*
 * This is used only to provide a block of storage to avoid memory fragmenting.
 * The information in this is accessible via the sensorRecord array below.
 */
static struct sensorReadings {
  struct tmp100 temperature[NTEMP];
  struct ltc2945 power[NPOWER];
  struct sfp    gbe;
  //struct pca8575 status[NSTATUS];
  //  struct firefly txrx[NFIREFLY];
  //#ifdef ACCESS_SRTM_TESTER
  //struct atca48V brick;
  //#endif
} sensorData;

/* 
 * Here is the array of structures for the actual sensors on the carrier.
 * It is used by both the I2C direct sensor access and by the IPMC indirect 
 * sensor access. For each sensor it contains the following information:
 *    1) The I2C address info
 *    2) The IPMC buffer offset information (NOT USED ON SRTM. BACKWARD COMPAT}
 *    3) Destination for the formatted and raw data. Always in the sensorData
 * Finally, in the I2C block are two fields which have meaning only for the 
 * case with the IPMC: Slot no. and IP address. Access functions are provided 
 * for these below. 
 *
 * Note that the bus mask macro assumes mux buses are numbered starting at 1 ala LArC.
 * in the 3rd field of the I2C address...
 */

#define SENSOR0 0
#define SENSOR1 1

#define MUXU64 0x70
#define MUXU2  0x72

static int nsensors=NTEMP+NPOWER+2;
static struct sensorRecord sensors[] = {
  /* The NTEMP temperature sensors. In the IPMC buffer, 16(byte) = 8(word) has FPGA temp. Skip it */
  {{SENSOR0,0,0,TMP100A,tmp100Init,tmp100Read,tmp100Format,0},{-1,0,0},&sensorData.temperature[0],"Tester Temp A"},
  {{SENSOR0,0,0,TMP100B,tmp100Init,tmp100Read,tmp100Format,0},{-1,0,0},&sensorData.temperature[1],"Tester Temp B"},
  {{SENSOR0,0,0,TMP100C,tmp100Init,tmp100Read,tmp100Format,0},{-1,0,0},&sensorData.temperature[2],"Tester Temp C"},
  {{SENSOR0,0,0,TMP100D,tmp100Init,tmp100Read,tmp100Format,0},{-1,0,0},&sensorData.temperature[3],"Tester Temp D"},
  /* And the power sensors */
  {{SENSOR1,MUXU64,1,VI_P12V, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[0],"Tester 12V"},
  {{SENSOR1,MUXU64,1,VI_P12VRTM, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[1],"SRTM 12V"},
  {{SENSOR1,MUXU64,1,VI_P3V3, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[2],"Tester 3.3V"},
  {{SENSOR1,MUXU64,2,VI_P1V2, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[3],"Tester 1.2V"},
  {{SENSOR1,MUXU64,2,VI_P0V85,ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[4],"Tester 0.85V"},
  {{SENSOR1,MUXU64,2,VI_P3V3a, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[5],"Tester 3.3V a"},
  {{SENSOR1,MUXU64,2,VI_P2V5, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[6],"Tester 2.5V"},
  {{SENSOR1,MUXU64,2,VI_P1V8, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[7],"Tester 1.8V"},
  {{SENSOR1,MUXU64,2,VI_P2V5a, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[8],"Tester 2.5V a"},
  {{SENSOR1,MUXU64,2,VI_P0V9, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[9],"Tester 0.9V"},
  {{SENSOR1,MUXU64,2,VI_P1V2a, ltc2945Init,ltc2945Read,ltc2945Format,0},{-1,0,0},&sensorData.power[10],"Tester 1.2V a"},
  /* sfp cage on SRTM tester */
  {{SENSOR1,MUXU2,1,SFP, sfpInit,sfpRead,sfpFormat,0},{-1,0,0},&sensorData.gbe,"SFP Insert"},
  {{0,0,0,0,0,0,0,0},{0,0,0},0,0}
};

/* ------------------------------------------------------------------------  */
/* ----  Here is the bus interface definition -----------------------------  */
/* ------------------------------------------------------------------------  */

#ifndef EXTERNAL_I2C
/* This is from the SRTM Zynq */
static struct i2cBus srtmBuses[] = {
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,I2CEnableRestart,I2CDisableRestart,-1,"/dev/i2c-0"},
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,I2CEnableRestart,I2CDisableRestart,-1,"/dev/i2c-1"}
};
#else
/* This is from e.g. a BeagleBone with eveything on bus 2. Put it in twice for consistency with Zynq defns above! */
static struct i2cBus srtmBuses[] = {
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,I2CEnableRestart,I2CDisableRestart,-1,"/dev/i2c-2"},
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,I2CEnableRestart,I2CDisableRestart,-1,"/dev/i2c-2"}
};
#endif

/* ------------------------------------------------------------------------  */
/* ------------------------------------------------------------------------  */
/* ------------------------------------------------------------------------  */

static int init=0, hasIPMC=0;

int sensorInit() {
  if( init ) return 0;
  i2cBusRegister(2,srtmBuses);
  sensorRegister(nsensors,sensors,hasIPMC);
  init = 1;
  return 0;
}

void sensorMonitorSRTM() {
}

#ifdef STANDALONE
int main( int argc, char **argv) {

  sensorInit();

  sensorMonitor();
  cJSON *top = cJSON_CreateObject();
  sensorFormat(top);
  cJSON_Print(top);

}
#endif
#endif
