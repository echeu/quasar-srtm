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
#include <iomanip>

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
#include "sensorInitSRTM_hw2.h"

#include "SysInfo.h"
#include "cJSON.h"
#include "json_provider.h"




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

  static int maxprint = 0;
  OpcUa_UInt32 spi[] = {
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0};

  static int first = 1;

  // ECC - Read from /dev/mem
  unsigned int bram_size = 0x8000;

  // This is the physical base address which is set by the constructor
  off_t bram_pbase = address();
  OpcUa_UInt32 *bram32_vptr;

  // see if we want to write to this address
  OpcUa_UInt32 setval = 0;
  getAddressSpaceLink()->getWriteRegValue(setval);

  // initialize write value here since I can't figure out how to do this in the constructor
  if (first) {
    saved_setval = setval;
    LOG(Log::INF) << "Register setval: " << std::hex << saved_setval;
    first = 0;

    // ECC - only do the sensor stuff for now
    sensorInitSRTM_hw2();
    LOG(Log::INF) << "Initialized sensor stuff.";
  }


  /* Get i2c data */
  sensorMonitor(0);

  // format sensor data. 
  // This is necessary to fill the appropriate arrays even though we don't access the cJSON data
  cJSON *top = cJSON_CreateObject();
  sysinfoFormat(top);
  sensorFormat(top);

  // ECC - open file for output --- This is temporary to check that it works
  FILE *json_file = fopen("SRTM-sensor.output.txt","w");
  fprintf(json_file, cJSON_Print(top));
  fclose(json_file);

  int tot_sensors = sensorCount();


  // print out the sensor data
  if (maxprint < 3) {
    std::cout << "Number of sensors: " << tot_sensors << std::endl;
    for (int i=0; i<tot_sensors; i++) {
      const struct sensorRecord *sensor_data;
      sensor_data = sensorGet(i);
      std::cout << "i: " << i << " name: " << sensor_data->name;
      void *buffer = sensor_data->valueBuffer;
      if (i<8) {
	float val1 = *static_cast<float *>(buffer);
	std::cout << " val1: " << val1 << std::endl;
      }
      else std::cout << std::endl;
    }
  }

  maxprint++;

  // For now just do the first 8 sensors...

  /******
  // Push the values to the OpcUa client display
  // See Sensor/src/firefly.c for definitions
  // And see Design/Design.xml for names of address space links
  getAddressSpaceLink()->setUserReg(val,OpcUa_Good);

  // fill SPI stuff
  getAddressSpaceLink()->setSPI00(spi[0],OpcUa_Good);
  getAddressSpaceLink()->setSPI01(spi[1],OpcUa_Good);
  getAddressSpaceLink()->setSPI02(spi[2],OpcUa_Good);
  getAddressSpaceLink()->setSPI03(spi[3],OpcUa_Good);
  getAddressSpaceLink()->setSPI04(spi[4],OpcUa_Good);
  getAddressSpaceLink()->setSPI05(spi[5],OpcUa_Good);
  getAddressSpaceLink()->setSPI06(spi[6],OpcUa_Good);
  getAddressSpaceLink()->setSPI07(spi[7],OpcUa_Good);
  getAddressSpaceLink()->setSPI08(spi[8],OpcUa_Good);
  getAddressSpaceLink()->setSPI09(spi[9],OpcUa_Good);
  getAddressSpaceLink()->setSPI10(spi[10],OpcUa_Good);
  getAddressSpaceLink()->setSPI11(spi[11],OpcUa_Good);
  getAddressSpaceLink()->setSPI12(spi[12],OpcUa_Good);
  getAddressSpaceLink()->setSPI13(spi[13],OpcUa_Good);
  getAddressSpaceLink()->setSPI14(spi[14],OpcUa_Good);
  getAddressSpaceLink()->setSPI15(spi[15],OpcUa_Good);
  getAddressSpaceLink()->setSPI16(spi[16],OpcUa_Good);
  getAddressSpaceLink()->setSPI17(spi[17],OpcUa_Good);
  getAddressSpaceLink()->setSPI18(spi[18],OpcUa_Good);
  getAddressSpaceLink()->setSPI19(spi[10],OpcUa_Good);
  getAddressSpaceLink()->setSPI20(spi[20],OpcUa_Good);
  getAddressSpaceLink()->setSPI21(spi[21],OpcUa_Good);
  getAddressSpaceLink()->setSPI22(spi[22],OpcUa_Good);
  getAddressSpaceLink()->setSPI23(spi[23],OpcUa_Good);
  getAddressSpaceLink()->setSPI24(spi[24],OpcUa_Good);
  getAddressSpaceLink()->setSPI25(spi[25],OpcUa_Good);
  getAddressSpaceLink()->setSPI26(spi[26],OpcUa_Good);
  getAddressSpaceLink()->setSPI27(spi[27],OpcUa_Good);
  getAddressSpaceLink()->setSPI28(spi[28],OpcUa_Good);
  getAddressSpaceLink()->setSPI29(spi[29],OpcUa_Good);
  getAddressSpaceLink()->setSPI30(spi[30],OpcUa_Good);
  getAddressSpaceLink()->setSPI31(spi[31],OpcUa_Good);
  getAddressSpaceLink()->setSPI32(spi[32],OpcUa_Good);
  getAddressSpaceLink()->setSPI33(spi[33],OpcUa_Good);
  getAddressSpaceLink()->setSPI34(spi[34],OpcUa_Good);
  getAddressSpaceLink()->setSPI35(spi[35],OpcUa_Good);
  getAddressSpaceLink()->setSPI36(spi[36],OpcUa_Good);
  getAddressSpaceLink()->setSPI37(spi[37],OpcUa_Good);

  // i2c stuff
  getAddressSpaceLink()->setFPGAuptime(vals[0],OpcUa_Good);
  getAddressSpaceLink()->setFPGAtemp(vals[1],OpcUa_Good);
  getAddressSpaceLink()->setFPGAvint(vals[2],OpcUa_Good);
  getAddressSpaceLink()->setFPGAvaux(vals[3],OpcUa_Good);
  getAddressSpaceLink()->setFPGAvbram(vals[4],OpcUa_Good);

  // Firefly 11 stuff
  getAddressSpaceLink()->setFF11txdisable(ff_vals[3],OpcUa_Good);
  getAddressSpaceLink()->setFF11tempC(ff_vals[13],OpcUa_Good);
  getAddressSpaceLink()->setFF11rxpower0(ff_vals[14],OpcUa_Good);
  getAddressSpaceLink()->setFF11rxpower1(ff_vals[15],OpcUa_Good);
  getAddressSpaceLink()->setFF11rxpower2(ff_vals[16],OpcUa_Good);
  getAddressSpaceLink()->setFF11rxpower3(ff_vals[17],OpcUa_Good);

  // Firefly 11 stuff
# define FF12off 19
  getAddressSpaceLink()->setFF12txdisable(ff_vals[3+FF12off],OpcUa_Good);
  getAddressSpaceLink()->setFF12tempC(ff_vals[13+FF12off],OpcUa_Good);
  getAddressSpaceLink()->setFF12rxpower0(ff_vals[14+FF12off],OpcUa_Good);
  getAddressSpaceLink()->setFF12rxpower1(ff_vals[15+FF12off],OpcUa_Good);
  getAddressSpaceLink()->setFF12rxpower2(ff_vals[16+FF12off],OpcUa_Good);
  getAddressSpaceLink()->setFF12rxpower3(ff_vals[17+FF12off],OpcUa_Good);

  // Compare values of FF11 txdisable and set value. Enable/disable if different.
  OpcUa_UInt32 setval_tx = 0;
  getAddressSpaceLink()->getFF11txwrite(setval_tx);
  OpcUa_UInt32 FF11_txdisable = ff_vals[3];
  if (FF11_txdisable != setval_tx) {
      
   // Loop over the four channels
    for (int ichan=0; ichan<nFFchan; ichan++) {
      int ibitset = (setval_tx >> ichan) & 1;
      int ibitval = (FF11_txdisable >> ichan) & 1;

      // only change enable if the two bits differ
      if (ibitset != ibitval) {
	if (ibitset == 0) fcChanEnable(16, ichan);
	else              fcChanDisable(16, ichan);
      }
    }
  }
  
  // Compare values of FF12 txdisable and set value. Enable/disable if different.
  getAddressSpaceLink()->getFF12txwrite(setval_tx);
  OpcUa_UInt32 FF12_txdisable = ff_vals[3+FF12off];
  if (FF12_txdisable != setval_tx) {
      
   // Loop over the four channels
    for (int ichan=0; ichan<nFFchan; ichan++) {
      int ibitset = (setval_tx >> ichan) & 1;
      int ibitval = (FF12_txdisable >> ichan) & 1;

      // only change enable if the two bits differ
      if (ibitset != ibitval) {
	if (ibitset == 0) fcChanEnable(15, ichan);
	else              fcChanDisable(15, ichan);
      }
    }
  }
  ************/

}

/* delegators for methods */

// 3333333333333333333333333333333333333333333333333333333333333333333333333
// 3     FULLY CUSTOM CODE STARTS HERE                                     3
// 3     Below you put bodies for custom methods defined for this class.   3
// 3     You can do whatever you want, but please be decent.               3
// 3333333333333333333333333333333333333333333333333333333333333333333333333

}


