
/*  �� Copyright CERN, 2015. All rights not expressly granted are reserved.

    The stub of this file was generated by quasar (https://github.com/quasar-team/quasar/)

    Quasar is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public Licence as published by
    the Free Software Foundation, either version 3 of the Licence.
    Quasar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public Licence for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Quasar.  If not, see <http://www.gnu.org/licenses/>.


 */


#include <Configuration.hxx> // TODO; should go away, is already in Base class for ages

#include <DRegs.h>
#include <ASRegs.h>
#include <string>

// ECC - include files for reading from /dev/mem (and maybe some extraneous things)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <iostream>

// ECC stuff for i2c
#include "malloc.h"
#include "xtypes.h"
#include "i2cBus.h"
#include "i2cDevUser.h"   /* Here "Dev" means /dev/i2c-X calls to access the bus. */
#include "tmp100.h"
#include "ltc2945.h"
#include "pca8575.h"
#include "firefly.h"
#include "ddr4.h"
#include "zynqOnBoard.h"

#include "sensorI2C.h"
#include "sensorData.h"
#include "sensorSRTM.h"

#include "SysInfo.h"
#include "cJSON.h"
#include "json_provider.h"


/* These are the i2C sensor counts. */
#define NTEMP 1      /* Temperatures from tmp100 */
#define NPOWER 9    /* VI ltc2945 */
#define NSTATUS 1    /* PCA power good register */
#define NFFFEX 4     /* 12 channel TX only modules */
#define NFFFELIX 2   /* 4 channel TX/RX modules */
#define NFIREFLY (NFFFEX+NFFFELIX)
#define NPSEQ 1
#define NDDR4 1
#define NZYNQ 1  /* For the internal Zynq sensors */


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

/*
 * This is used only to provide a block of storage to avoid memory fragmenting.
 * The information in this is accessible via the sensorRecord array below.
 */
static struct sensorReadings {
  struct tmp100 temperature[NTEMP];
  struct ltc2945 power[NPOWER];
  struct pca8575 status[NSTATUS];
  struct firefly12 txrx12[NFIREFLY-2];
  struct firefly4 txrx4[2];
#ifdef ACCESS_SRTM_TESTER
  struct atca48V brick;
#endif
  struct ddr4 ddr4;
  struct zynqOnBoard zynqInternal;
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

static int nsensors=NTEMP+NPOWER+NSTATUS+NFIREFLY+NPSEQ+NDDR4+NZYNQ;
//static int nsensors=NZYNQ;
static struct sensorRecord sensors[] = {
  /* ECC - changing {-1,0,0} to {1,0,0} since it causes compiler errors */
  /* The NTEMP temperature sensors. In the IPMC buffer, 16(byte) = 8(word) has FPGA temp. Skip it */
  {{SENSOR0,0,0,TMP100A,tmp100Init,tmp100Read,tmp100Format,0},{1,0,0},&sensorData.temperature[0],"SRTM Temp"},
  /* And the power sensors */
  {{SENSOR0,0,0,TMPZYNQ, ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_zynq_diode},{1,0,0},&sensorData.power[0],"Zynq Diode"},
  {{SENSOR1,0,0,VI_P1V8, ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4628},{1,0,0},&sensorData.power[1],"SRTM1.8V (U12)"},
  {{SENSOR1,0,0,VI_P3V3, ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4628},{1,0,0},&sensorData.power[2],"SRTM3.3V (U11)"},
  {{SENSOR1,0,0,VI_P2V5, ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4628},{1,0,0},&sensorData.power[3],"SRTM2.5V (U12)"},
  {{SENSOR1,0,0,VI_P1V2, ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4630},{1,0,0},&sensorData.power[4],"SRTM1.2V (U1)"},
  {{SENSOR1,0,0,VI_P0V9, ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4628},{1,0,0},&sensorData.power[5],"SRTM0.9V (U9)"},
  {{SENSOR1,0,0,VI_P0V85,ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4630},{1,0,0},&sensorData.power[6],"SRTM0.85V (U1)"},
  {{SENSOR1,0,0,VI_P1V2DDR, ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4628},{1,0,0},&sensorData.power[7],"SRTM DDR4 1.2V (U9)"},
  {{SENSOR1,0,0,VI_P3V3FF,ltc2945Init,ltc2945Read,ltc2945Format,(void* (*)())c_ltm4628},{1,0,0},&sensorData.power[8],"SRTM Firefly 3.3V (U11)"},
  {{SENSOR0,0,0,PSTATUS,pca8575Init,pca8575Read,pca8575Format,0},{1,0,0},&sensorData.status[0],"SRTM Power Status"},

  {{SENSOR1,FFOutputReg,0x80 | 0, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12,0},{1,0,0},&sensorData.txrx12[0],"FF-TX4"},
  {{SENSOR1,FFOutputReg,0x80 | 2, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12,0},{1,0,0},&sensorData.txrx12[1],"FF-TX3"},
  {{SENSOR1,FFOutputReg,0x80 | 4, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12,0},{1,0,0},&sensorData.txrx12[2],"FF-TX2"},
  {{SENSOR1,FFOutputReg,0x80 | 6, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12,0},{1,0,0},&sensorData.txrx12[3],"FF-TX1"},
  {{SENSOR1,FFOutputReg,0x80 | 12, FF10TXRX, fireflyInit4,fireflyRead4,fireflyFormat4,0},{1,0,0},&sensorData.txrx4[0],"FF-TXRX12"},
  {{SENSOR1,FFOutputReg,0x80 | 14, FF10TXRX, fireflyInit4,fireflyRead4,fireflyFormat4,0},{1,0,0},&sensorData.txrx4[1],"FF-TXRX11"},
  {{SENSOR1,0,0,DDR4ADDR,ddr4Init,ddr4Read,ddr4Format,0},{1,0,0},&sensorData.ddr4,"DDR4"},
  {{0,0,0,ZYNQADDR,zynqOnBoardInit,zynqOnBoardRead,zynqOnBoardFormat,0},{1,0,0},&sensorData.zynqInternal,"ZynqInternal"},
  // ECC - The following line was used when we were only looking at zynq data
  //{{0,0,0,ZYNQADDR,zynqOnBoardInit,zynqOnBoardRead,zynqOnBoardFormat,0},{1,0,0},&sensorData.zynqInternal,"ZynqInternal"},
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


namespace Device
{
// 1111111111111111111111111111111111111111111111111111111111111111111111111
// 1     GENERATED CODE STARTS HERE AND FINISHES AT SECTION 2              1
// 1     Users don't modify this code!!!!                                  1
// 1     If you modify this code you may start a fire or a flood somewhere,1
// 1     and some human being may possible cease to exist. You don't want  1
// 1     to be charged with that!                                          1
// 1111111111111111111111111111111111111111111111111111111111111111111111111


//static off_t bram_pbase;
static OpcUa_UInt32 saved_setval;



// 2222222222222222222222222222222222222222222222222222222222222222222222222
// 2     SEMI CUSTOM CODE STARTS HERE AND FINISHES AT SECTION 3            2
// 2     (code for which only stubs were generated automatically)          2
// 2     You should add the implementation but dont alter the headers      2
// 2     (apart from constructor, in which you should complete initializati2
// 2     on list)                                                          2
// 2222222222222222222222222222222222222222222222222222222222222222222222222

/* sample ctr */
DRegs::DRegs (
    const Configuration::Regs& config,
    Parent_DRegs* parent
):
    Base_DRegs( config, parent)

    /* fill up constructor initialization list here */
{
    /* fill up constructor body here */

	// initialize the base address from the configuration file
	LOG(Log::INF) << "Register address: " << std::hex << config.address();

}

/* sample dtr */
DRegs::~DRegs ()
{
}

/* delegates for cachevariables */

// update output from /dev/mem
void DRegs::update() {

  OpcUa_UInt32 val = 0;
  static int first = 1;

  // ECC - Read from /dev/mem
  unsigned int bram_size = 0x8000;

  // This is the physical base address which is set by the constructor
  off_t bram_pbase = address();
  OpcUa_UInt64 *bram64_vptr;
  int fd;

  // see if we want to write to this address
  OpcUa_UInt32 setval = 0;
  getAddressSpaceLink()->getWriteRegValue(setval);

  // initialize write value here since I can't figure out how to do this in the constructor
  if (first) {
    saved_setval = setval;
    LOG(Log::INF) << "Register setval: " << std::hex << saved_setval;
    first = 0;

    // i2c initialization
    int hasIPMC = 0;
    i2cBusRegister(2,srtmBuses);
    sensorRegister(nsensors,sensors,hasIPMC);
    LOG(Log::INF) << "Initialized i2c buses.";
  }


	// Map the BRAM physical address into user space getting a virtual address for it
  if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {
    bram64_vptr = (OpcUa_UInt64 *)mmap(NULL, bram_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, bram_pbase);

    // if the value has changed, then we want to write it to memory
    if (setval != saved_setval) {
      LOG(Log::INF) << "Changing register value to: " << std::hex << setval;
      bram64_vptr[0] = setval;
      saved_setval = setval;
    }

    val = bram64_vptr[0];
    close(fd);
  }

  /* Get i2c data */
  sensorMonitor(0);

  // format sensor data. 
  // This is necessary to fill the appropriate arrays even though we don't access the cJSON data
  cJSON *top = cJSON_CreateObject();
  sysinfoFormat(top);
  sensorFormat(top);

  // Print out i2c values using JSON 
  //printf("%s\n",cJSON_Print(top));

  // ECC - i2c stuff
  //     - try to get information using array access of sensors
  static int maxprint = 0;

  /* This isn't really necessary
  int nsensor = sensorCount();
  for (int is=0; is<nsensor; is++) {
    const struct sensorRecord *record = sensorGet(is);
    int isval = *(int*)record->valueBuffer;
    if (maxprint < 5) std::cout << "isval: " << isval << " name: " << record->name << std::endl;
  }
  */

  // Get Zynq values and Firefly values (see Sensor/src/zynqOnBoard.c and Sensor/src/firefly.c).
  int maxvals=40;
  double vals[maxvals], ff_vals[maxvals];
  char *names[maxvals], *ff_names[maxvals];
  getzynqvals(vals, names);
  getFFvals(ff_vals, ff_names);
  if (maxprint < 5) {
    std::cout << std::endl << "iteration: " << maxprint << std::endl;
    for (int iv=0; iv<35; iv++) std::cout << "zynq vals (" << iv << "): " << names[iv] << " " << vals[iv] << std::endl;

    // Print out firefly values here
    std::cout << std::endl;
    for (int iv=0; iv<38; iv++) {
      std::cout << "FF vals: " << ff_names[iv] << " " << ff_vals[iv] << std::endl;
    }
  }
  maxprint++;

  // Push the values to the OpcUa client display
  // See Sensor/src/firefly.c for definitions
  // And see Design/Design.xml for names of address space links
  getAddressSpaceLink()->setUserReg(val,OpcUa_Good);
  getAddressSpaceLink()->setFPGAuptime(vals[0],OpcUa_Good);
  getAddressSpaceLink()->setFPGAtemp(vals[1],OpcUa_Good);
  getAddressSpaceLink()->setFPGAvint(vals[2],OpcUa_Good);
  getAddressSpaceLink()->setFPGAvaux(vals[3],OpcUa_Good);
  getAddressSpaceLink()->setFPGAvbram(vals[4],OpcUa_Good);

  // Firefly stuff
  getAddressSpaceLink()->setFireflytxdisable(ff_vals[3],OpcUa_Good);
  getAddressSpaceLink()->setFireflytempC(ff_vals[13],OpcUa_Good);
  getAddressSpaceLink()->setFireflyrxpower0(ff_vals[14],OpcUa_Good);
  getAddressSpaceLink()->setFireflyrxpower1(ff_vals[15],OpcUa_Good);
  getAddressSpaceLink()->setFireflyrxpower2(ff_vals[16],OpcUa_Good);
  getAddressSpaceLink()->setFireflyrxpower3(ff_vals[17],OpcUa_Good);

  // Compare values of FF11 txdisable and set value. Enable/disable if different.
  OpcUa_UInt32 setval_tx = 0;
  getAddressSpaceLink()->getWriteValueF11TX(setval_tx);
  UInt32 F11_txdisable = ff_vals[3];
  if (F11_txdisable != setval_tx) {
    if (setval_tx == 0) {
      disableFF11();
    }
    else {
      enableFF11();
    }
  }
  

}
/* delegators for methods */

// 3333333333333333333333333333333333333333333333333333333333333333333333333
// 3     FULLY CUSTOM CODE STARTS HERE                                     3
// 3     Below you put bodies for custom methods defined for this class.   3
// 3     You can do whatever you want, but please be decent.               3
// 3333333333333333333333333333333333333333333333333333333333333333333333333

}
