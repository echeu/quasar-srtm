/*
 * sensorSRTM.c
 *
 *  Created on: 
 *      Author: 
 *
 *   This is the master sensor data program for the HL-LHC SRTM i2c sensor access.
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

#define ACCESS_SRTM
#ifdef ACCESS_SRTM

#include "malloc.h"
#include "string.h"

#include "xtypes.h"

#include "i2cBus.h"
#include "i2cDevUser.h"   /* Here "Dev" means /dev/i2c-X calls to access the bus. */

#include "tmp100.h"
#include "ltc2945.h"
#include "pca8575.h"
#include "firefly.h"
#include "ddr4.h"
#include "zynqOnBoard.h"
#include "zynqPL.h"
#include "sfp.h"
#include "ipmcSensorBlock.h"

#include "sensorI2C.h"
#include "sensorData.h"
#include "sensorInitSRTM_hw2.h"

#include "SysInfo.h"
#include "cJSON.h"
#include "json_provider.h"

/* These are the i2C sensor counts. */
#ifdef USE_SHARED_I2C /* this might cause crashes */
#define NTEMP 1      /* Temperatures from tmp100 */
#define NPOWER 9    /* VI ltc2945 */
#define NSTATUS 1    /* PCA power good register */
#else
#define NTEMP 0      /* Temperatures from tmp100 */
#define NPOWER 8    /* VI ltc2945 */
#define NSTATUS 0    /* PCA power good register */
#endif
#define NFFMON 1     /* 4 channel TX/RX module for alt mon path */
#define NFFFELIX 2   /* 4 channel TX/RX modules */
#define NFIREFLY (NFFMON+NFFFELIX)
#define NSFPEN 1     /* Number of SFP inserts with I2C enable */
#define NPSEQ 1
#define NDDR4 1
//#define NZYNQ 2  /* Pre 2024.1 SRTM, the internal Zynq PS and PL side sensors */
#define NZYNQ 1 /* Starting with 2024.1 SRTM, only PS side is used */
#define NIPMC 1 /* This is the (optional?) data from the IPMC via I2C.*/

/* --- SRTM sensors -----------------------------------------------------------
 * First, we do the devices on the RTM_SENSOR bus. This is shared with the 
 * Zone 3 I2C from the LASP/SRTM tester card.
 *
 *  12 V DC-DC temperature
 */
#define TMP100A  0x4F /* U2 */

/* This is an ltc2945, but it's connected to the Zynq temperature diode,
   so it's really a temperature sensor. Does this need special handling? 
 */
#define TMPZYNQ 0x6A

/* This is the PCA8575 register with the power status bits */
#define PSTATUS 0x20

/* Now we do the RTM bus. This is private to the SRTM Zynq.
 * It has clocks, DDR, firefly (and enable) and VI sensors
 * Symbolic names for each current/voltage sensor. 
*/
#define VI_P1V8    0x67
#define VI_P3V3    0x6F
#define VI_P1V2    0x6B
#define VI_P0V9    0x6A
#define VI_P2V5    0x6D
#define VI_P0V85   0x6C
#define VI_P3V3FF  0x69 /* For the Fireflies only */
#define VI_P1V2DDR 0x6E

/* The DDR4 module */
#define DDR4ADDR 0x51

/* The Zynq internal. No address actually needed */
#define ZYNQADDR 0xFF

/* ---- Now the sensors on the (optional) tester card -------------------------
 */
#define VI_12V_BRICK  0x67 /* U8 - 12V brick */

#define SFP 0x50

/*
 * This is used only to provide a block of storage to avoid memory fragmenting.
 * The information in this is accessible via the sensorRecord array below.
 */
static struct sensorReadings {
  struct tmp100 temperature[NTEMP];
  struct ltc2945 power[NPOWER];
  struct pca8575 status[NSTATUS];
  struct firefly4 ffMon[NFFMON];
  struct firefly4 ffFelix[NFFFELIX];
  struct sfp ttcLink;
  //#ifdef ACCESS_SRTM_TESTER
  //struct atca48V brick;
  //#endif
  struct ddr4 ddr4;
  struct zynqOnBoard zynqInternal;
  struct zynqPL zynqPLInternal;
  struct ipmcDataRecord ipmcInternal;
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

static int nsensors=NTEMP+NPOWER+NSTATUS+NFIREFLY+NPSEQ+NDDR4+NZYNQ+NIPMC;
static struct sensorRecord sensors[] = {
#ifdef USE_SHARED_I2C /* this might cause crashes */
  /* The NTEMP temperature sensors. In the IPMC buffer, 16(byte) = 8(word) has FPGA temp. Skip it */
  {{SENSOR0,0,0,TMP100A,tmp100Init,tmp100Read,tmp100Format,0},{-1,0,0},&sensorData.temperature[0],"SRTM Temp"},
#endif
  /* And the power sensors */
  {{SENSOR1,0,0,VI_P1V8, ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4628},{-1,0,0},&sensorData.power[0],"SRTM 1.8V (U12)","_v1p8"},
  {{SENSOR1,0,0,VI_P3V3, ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4628},{-1,0,0},&sensorData.power[1],"SRTM 3.3V (U11)","_v3p3"},
  {{SENSOR1,0,0,VI_P2V5, ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4628},{-1,0,0},&sensorData.power[2],"SRTM 2.5V (U12)","_v2p5"},
  {{SENSOR1,0,0,VI_P1V2, ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4630},{-1,0,0},&sensorData.power[3],"SRTM 1.2V (U1)","_v1p2"},
  {{SENSOR1,0,0,VI_P0V9, ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4628},{-1,0,0},&sensorData.power[4],"SRTM 0.9V (U9)","_v0p9"},
  {{SENSOR1,0,0,VI_P0V85,ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4630},{-1,0,0},&sensorData.power[5],"SRTM 0.85V (U1)","_v0p85"},
  {{SENSOR1,0,0,VI_P1V2DDR, ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4628},{-1,0,0},&sensorData.power[6],"SRTM DDR4 1.2V (U9)","_pddr"},
  {{SENSOR1,0,0,VI_P3V3FF,ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_ltm4628},{-1,0,0},&sensorData.power[7],"SRTM Firefly 3.3V (U11)","_pff"},
#ifdef USE_SHARED_I2C /* this might cause crashes */
  {{SENSOR0,0,0,TMPZYNQ, ltc2945Init,ltc2945Read,ltc2945Format,(void*)c_zynq_diode},{-1,0,0},&sensorData.power[8],"Zynq Diode","_z"},
  {{SENSOR0,0,0,PSTATUS,pca8575Init,pca8575Read,pca8575Format,0},{-1,0,0},&sensorData.status[0],"SRTM Power Status"},
#endif  
  {{SENSOR1,FFIOReg,0x80 | 2, FF25TXRX, fireflyInit4,fireflyRead4,fireflyFormat4,0},{-1,0,0},&sensorData.ffMon[0],"FF-TXRX13","13"},
  {{SENSOR1,FFIOReg,0x80 | 4, FF25TXRX, fireflyInit4,fireflyRead4,fireflyFormat4,0},{-1,0,0},&sensorData.ffFelix[0],"FF-TXRX12","12"},
  {{SENSOR1,FFIOReg,0x80 | 6, FF25TXRX, fireflyInit4,fireflyRead4,fireflyFormat4,0},{-1,0,0},&sensorData.ffFelix[1],"FF-TXRX11","11"},
  {{SENSOR1,FFIOReg,0x80 | 0, SFP, sfpInit,sfpRead,sfpFormat,0},{-1,0,0},&sensorData.ttcLink,"TTC"},
  {{SENSOR1,0,0,DDR4ADDR,ddr4Init,ddr4Read,ddr4Format,0},{-1,0,0},&sensorData.ddr4,"DDR4"},
  {{0,0,0,ZYNQADDR,zynqOnBoardInit,zynqOnBoardRead,zynqOnBoardFormat,0},{-1,0,0},&sensorData.zynqInternal,"ZynqInternal"}, /* Psuedo sensor. It's a group */
//  {{0,0,0,ZYNQADDR,zynqPLInit,zynqPLRead,zynqPLFormat,0},{-1,0,0},&sensorData.zynqPLInternal,"ZynqPLInternal"},  /* Psuedo sensor. It's a group */
  {{0,0,0,ZYNQADDR,ipmcSensorBlockInit,ipmcSensorBlockRead,ipmcSensorBlockFormat,0},{-1,0,0},&sensorData.ipmcInternal,"IpmcInternal"}, /* Psuedo sensor. It's a group */
  {{0,0,0,0,0,0,0,0},{0,0,0},0,0}
};

/* ------------------------------------------------------------------------  */
/* ----  Here is the bus interface definition -----------------------------  */
/* ------------------------------------------------------------------------  */

#ifndef EXTERNAL_I2C
static struct i2cBus srtmBuses[] = {
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,0,0,-1,"/dev/i2c-0"},
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,0,0,-1,"/dev/i2c-1"}
};
#else
/* This is from a e.g. BeagleBone with eveything on bus 2. Put in a double entry to avoid needing
 * to reconfigure the sensors[] array above.
 */
static struct i2cBus srtmBuses[] = {
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,0,0,-1,"/dev/i2c-2"},
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,0,0,-1,"/dev/i2c-2"},
};
#endif

/* ------------------------------------------------------------------------  */
/* ------------------------------------------------------------------------  */
/* ------------------------------------------------------------------------  */

static int init=0, hasIPMC=0;

int getNSensors_hw2() {
  return nsensors;
}

struct sensorRecord* getSensorRecord_hw2() {
  return sensors;
}

int sensorInitSRTM_hw2() {
  if( init ) return 0;
  i2cBusRegister(2,srtmBuses);
  sensorRegister(nsensors,sensors,hasIPMC);
  setHWEFuse(0x06200000); /* Make a fake V2 SRTM efuse value. */
  init = 1;
  return 0;
}
#endif
