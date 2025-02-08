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

}

/* sample dtr */

DRegs::~DRegs ()
{
}


/* delegates for cachevariables */

// update output from /dev/mem
void DRegs::update() {

  static int maxprint = 0;
  static int first = 1;

  // initialize write value here since I can't figure out how to do this in the constructor
  if (first) {
    first = 0;

    // ECC - only do the sensor stuff for now
    sensorInitSRTM_hw2();
    LOG(Log::INF) << "Initialized sensor stuff.";
  }


  /* Get i2c data */
  sensorMonitor(0);

  // format sensor data. 
  cJSON *top = cJSON_CreateObject();
  sysinfoFormat(top);
  sensorFormat(top);

  // print out the sensor data
  if (maxprint < 3) {

    // ECC - open file for output --- This is temporary to check that it works
    FILE *json_file = fopen("SRTM-sensor.output.txt","w");
    char *string_json = NULL;
    string_json = cJSON_Print(top);
    fprintf(json_file, string_json);
    fclose(json_file);

    // write to std out
    std::cout << string_json << std::endl;
  }
  maxprint++;

  // get header information
  extract_header(top);

  cJSON *sensor_json;
  sensor_json = cJSON_GetObjectItem(top, "sensor");
  // Get the ltc values
  if (sensor_json) extract_ltc(sensor_json);

  // Get firefly data
  if (sensor_json) extract_firefly(11, sensor_json);
  if (sensor_json) extract_firefly(12, sensor_json);
  if (sensor_json) extract_firefly(13, sensor_json);
  
  // Get FPGA values
  if (sensor_json) extract_fpga(sensor_json);
  
  // ECC - be sure to delete cJSON object
  cJSON_Delete(top);

}

// get header data from JSON object
void DRegs::extract_header(cJSON *top) {
  cJSON *hwid_json = NULL;
  cJSON *versions_json = NULL;
  cJSON *fwvers_json = NULL;
  cJSON *swvers_json = NULL;
  double hwid = -99, fwvers = -99, swvers = -99;
  
  // unpack header data 
  hwid_json = cJSON_GetObjectItem(top, "hwid");
  versions_json = cJSON_GetObjectItem(top, "versions");
  if (versions_json) fwvers_json = cJSON_GetObjectItem(versions_json, "fwvers");
  if (versions_json) swvers_json = cJSON_GetObjectItem(versions_json, "swvers");

  // get values
  if (hwid_json) hwid = hwid_json->valuedouble;
  if (fwvers_json) fwvers = fwvers_json->valuedouble;
  if (swvers_json) swvers = swvers_json->valuedouble;

  // Link to OpcUA
  getAddressSpaceLink()->setHwid(hwid,OpcUa_Good);
  getAddressSpaceLink()->setFwvers(fwvers,OpcUa_Good);
  getAddressSpaceLink()->setSwvers(swvers,OpcUa_Good);

}

// Get ltc values from JSON object
void DRegs::extract_ltc(cJSON *sensor_json) {

  cJSON *ltc_v1p8_json = NULL;
  cJSON *ltc_v3p3_json = NULL;
  cJSON *ltc_v2p5_json = NULL;
  cJSON *ltc_v1p2_json = NULL;
  cJSON *ltc_v0p9_json = NULL;
  cJSON *ltc_v0p85_json = NULL;
  cJSON *ltc_pddr_json = NULL;
  cJSON *ltc_pff_json = NULL;

  // voltages, currents and temps
  double ltc_v1p8, ltc_v3p3, ltc_v2p5, ltc_v1p2, ltc_v0p9, ltc_v0p85, ltc_pddr, ltc_pff;
  double ltc_v1p8_cur, ltc_v3p3_cur, ltc_v2p5_cur, ltc_v1p2_cur, ltc_v0p9_cur, ltc_v0p85_cur, ltc_pddr_cur, ltc_pff_cur;
  double ltc_v1p8_tmp, ltc_v3p3_tmp, ltc_v2p5_tmp, ltc_v1p2_tmp, ltc_v0p9_tmp, ltc_v0p85_tmp, ltc_pddr_tmp, ltc_pff_tmp;


  // unpack data from the ltc2945 device
  if (sensor_json) ltc_v1p8_json = cJSON_GetObjectItem(sensor_json, "ltc2945_v1p8");
  if (sensor_json) ltc_v3p3_json = cJSON_GetObjectItem(sensor_json, "ltc2945_v3p3");
  if (sensor_json) ltc_v2p5_json = cJSON_GetObjectItem(sensor_json, "ltc2945_v2p5");
  if (sensor_json) ltc_v1p2_json = cJSON_GetObjectItem(sensor_json, "ltc2945_v1p2");
  if (sensor_json) ltc_v0p9_json = cJSON_GetObjectItem(sensor_json, "ltc2945_v0p9");
  if (sensor_json) ltc_v0p85_json = cJSON_GetObjectItem(sensor_json, "ltc2945_v0p85");
  if (sensor_json) ltc_pddr_json = cJSON_GetObjectItem(sensor_json, "ltc2945_pddr");
  if (sensor_json) ltc_pff_json = cJSON_GetObjectItem(sensor_json, "ltc2945_pff");

  // get the voltages
  if (ltc_v1p8_json) ltc_v1p8 = cJSON_GetArrayItem(ltc_v1p8_json,2)->valuedouble;
  if (ltc_v3p3_json) ltc_v3p3 = cJSON_GetArrayItem(ltc_v3p3_json,2)->valuedouble;
  if (ltc_v2p5_json) ltc_v2p5 = cJSON_GetArrayItem(ltc_v2p5_json,2)->valuedouble;
  if (ltc_v1p2_json) ltc_v1p2 = cJSON_GetArrayItem(ltc_v1p2_json,2)->valuedouble;
  if (ltc_v0p9_json) ltc_v0p9 = cJSON_GetArrayItem(ltc_v0p9_json,2)->valuedouble;
  if (ltc_v0p85_json)ltc_v0p85 = cJSON_GetArrayItem(ltc_v0p85_json,2)->valuedouble;
  if (ltc_pddr_json) ltc_pddr = cJSON_GetArrayItem(ltc_pddr_json,2)->valuedouble;
  if (ltc_pff_json)  ltc_pff = cJSON_GetArrayItem(ltc_pff_json,2)->valuedouble;

  // get the currents
  if (ltc_v1p8_json) ltc_v1p8_cur = cJSON_GetArrayItem(ltc_v1p8_json,3)->valuedouble;
  if (ltc_v3p3_json) ltc_v3p3_cur = cJSON_GetArrayItem(ltc_v3p3_json,3)->valuedouble;
  if (ltc_v2p5_json) ltc_v2p5_cur = cJSON_GetArrayItem(ltc_v2p5_json,3)->valuedouble;
  if (ltc_v1p2_json) ltc_v1p2_cur = cJSON_GetArrayItem(ltc_v1p2_json,3)->valuedouble;
  if (ltc_v0p9_json) ltc_v0p9_cur = cJSON_GetArrayItem(ltc_v0p9_json,3)->valuedouble;
  if (ltc_v0p85_json)ltc_v0p85_cur = cJSON_GetArrayItem(ltc_v0p85_json,3)->valuedouble;
  if (ltc_pddr_json) ltc_pddr_cur = cJSON_GetArrayItem(ltc_pddr_json,3)->valuedouble;
  if (ltc_pff_json)  ltc_pff_cur = cJSON_GetArrayItem(ltc_pff_json,3)->valuedouble;

  // get the temperatures
  if (ltc_v1p8_json) ltc_v1p8_tmp = cJSON_GetArrayItem(ltc_v1p8_json,4)->valuedouble;
  if (ltc_v3p3_json) ltc_v3p3_tmp = cJSON_GetArrayItem(ltc_v3p3_json,4)->valuedouble;
  if (ltc_v2p5_json) ltc_v2p5_tmp = cJSON_GetArrayItem(ltc_v2p5_json,4)->valuedouble;
  if (ltc_v1p2_json) ltc_v1p2_tmp = cJSON_GetArrayItem(ltc_v1p2_json,4)->valuedouble;
  if (ltc_v0p9_json) ltc_v0p9_tmp = cJSON_GetArrayItem(ltc_v0p9_json,4)->valuedouble;
  if (ltc_v0p85_json)ltc_v0p85_tmp = cJSON_GetArrayItem(ltc_v0p85_json,4)->valuedouble;
  if (ltc_pddr_json) ltc_pddr_tmp = cJSON_GetArrayItem(ltc_pddr_json,4)->valuedouble;
  if (ltc_pff_json)  ltc_pff_tmp = cJSON_GetArrayItem(ltc_pff_json,4)->valuedouble;

  // link the data to OpcUa
  getAddressSpaceLink()->setSRTM_v00(ltc_v1p8,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_v01(ltc_v3p3,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_v02(ltc_v2p5,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_v03(ltc_v1p2,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_v04(ltc_v0p9,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_v05(ltc_v0p85,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_v06(ltc_pddr,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_v07(ltc_pff,OpcUa_Good);

  getAddressSpaceLink()->setSRTM_c00(ltc_v1p8_cur,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_c01(ltc_v3p3_cur,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_c02(ltc_v2p5_cur,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_c03(ltc_v1p2_cur,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_c04(ltc_v0p9_cur,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_c05(ltc_v0p85_cur,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_c06(ltc_pddr_cur,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_c07(ltc_pff_cur,OpcUa_Good);

  getAddressSpaceLink()->setSRTM_t00(ltc_v1p8_tmp,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_t01(ltc_v3p3_tmp,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_t02(ltc_v2p5_tmp,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_t03(ltc_v1p2_tmp,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_t04(ltc_v0p9_tmp,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_t05(ltc_v0p85_tmp,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_t06(ltc_pddr_tmp,OpcUa_Good);
  getAddressSpaceLink()->setSRTM_t07(ltc_pff_tmp,OpcUa_Good);
}

// Code to extract the information for FF13
// Unfortunately the SpaceLink code needs to be hardwired so it is not easily simplified
void DRegs::extract_firefly(int ffnumber, cJSON *sensor_json) {

  cJSON *ff_json = NULL;
  cJSON *present_json = NULL;
  cJSON *status_json = NULL;
  cJSON *txdisable_json = NULL;
  cJSON *cdrenable_json = NULL;
  cJSON *cdrrate_json = NULL;
  cJSON *cdrlol_json = NULL;
  cJSON *los_json = NULL;
  cJSON *txfault_json = NULL;
  cJSON *tempfault_json = NULL;
  cJSON *voltfault_json = NULL;
  cJSON *powerfault_json = NULL;
  cJSON *uptime_json = NULL;
  cJSON *tempC_json = NULL;
  cJSON *rxpower_json = NULL;
  cJSON *rxpower0_json, *rxpower1_json, *rxpower2_json, *rxpower3_json;
  cJSON *id_json = NULL;
  cJSON *model_json = NULL;
  cJSON *serial_json = NULL;
  cJSON *fwversion_json = NULL;

  double present = -99;
  double status = -99;
  double txdisable = -99;
  double cdrenable = -99;
  double cdrrate = -99;
  double cdrlol = -99;
  double los = -99;
  double txfault = -99;
  double tempfault = -99;
  double voltfault = -99;
  double powerfault = -99;
  double tempC = -99;
  double uptime = -99;
  double rxpower_0, rxpower_1, rxpower_2, rxpower_3;
  double id;
  char *model, *serial, *fwversion;

  // construct firefly name
  std::string ffname = "firefly" + std::to_string(ffnumber);
  int len = ffname.length()+1;
  char cffname[len];
  strcpy(cffname, ffname.c_str());
  
  // Get data from JSON construct
  if (sensor_json) ff_json = cJSON_GetObjectItem(sensor_json, cffname);
  if (ff_json) present_json = cJSON_GetObjectItem(ff_json, "present");
  if (ff_json) status_json = cJSON_GetObjectItem(ff_json, "status");
  if (ff_json) txdisable_json = cJSON_GetObjectItem(ff_json, "txdisable");
  if (ff_json) cdrenable_json = cJSON_GetObjectItem(ff_json, "cdrenable");
  if (ff_json) cdrrate_json = cJSON_GetObjectItem(ff_json, "cdrrate");
  if (ff_json) cdrlol_json = cJSON_GetObjectItem(ff_json, "cdrlol");
  if (ff_json) los_json = cJSON_GetObjectItem(ff_json, "los");
  if (ff_json) txfault_json = cJSON_GetObjectItem(ff_json, "txfault");
  if (ff_json) tempfault_json = cJSON_GetObjectItem(ff_json, "tempfault");
  if (ff_json) voltfault_json = cJSON_GetObjectItem(ff_json, "voltfault");
  if (ff_json) powerfault_json = cJSON_GetObjectItem(ff_json, "powerfault");
  if (ff_json) uptime_json = cJSON_GetObjectItem(ff_json, "uptime");
  if (ff_json) tempC_json = cJSON_GetObjectItem(ff_json, "tempC");
  if (ff_json) rxpower_json = cJSON_GetObjectItem(ff_json, "rxpower");
  if (ff_json) id_json = cJSON_GetObjectItem(ff_json, "id");
  if (ff_json) model_json = cJSON_GetObjectItem(ff_json, "model");
  if (ff_json) serial_json = cJSON_GetObjectItem(ff_json, "serial");
  if (ff_json) fwversion_json = cJSON_GetObjectItem(ff_json, "fwversion");

  if (present_json) present = present_json->valuedouble;
  if (status_json) status = status_json->valuedouble;
  if (txdisable_json) txdisable = txdisable_json->valuedouble;
  if (cdrenable_json) cdrenable = cdrenable_json->valuedouble;
  if (cdrrate_json) cdrrate = cdrrate_json->valuedouble;
  if (cdrlol_json) cdrlol = cdrlol_json->valuedouble;
  if (los_json) los = los_json->valuedouble;
  if (txfault_json) txfault = txfault_json->valuedouble;
  if (tempfault_json) tempfault = tempfault_json->valuedouble;
  if (voltfault_json) voltfault = voltfault_json->valuedouble;
  if (powerfault_json) powerfault = powerfault_json->valuedouble;
  if (uptime_json) uptime = uptime_json->valuedouble;
  if (tempC_json) tempC = tempC_json->valuedouble;
  if (rxpower_json) rxpower0_json = cJSON_GetArrayItem(rxpower_json,0);
  if (rxpower_json) rxpower1_json = cJSON_GetArrayItem(rxpower_json,1);
  if (rxpower_json) rxpower2_json = cJSON_GetArrayItem(rxpower_json,2);
  if (rxpower_json) rxpower3_json = cJSON_GetArrayItem(rxpower_json,3);
  if (rxpower0_json) rxpower_0 = rxpower0_json->valuedouble;
  if (rxpower1_json) rxpower_1 = rxpower1_json->valuedouble;
  if (rxpower2_json) rxpower_2 = rxpower2_json->valuedouble;
  if (rxpower3_json) rxpower_3 = rxpower3_json->valuedouble;
  if (id_json) id = id_json->valuedouble;
  if (model_json) model = model_json->valuestring;
  if (serial_json) serial = serial_json->valuestring;
  if (fwversion_json) fwversion = fwversion_json->valuestring;


  // link data to OpcUA
  if (ffnumber == 11) {
    getAddressSpaceLink()->setFF11_present(present,OpcUa_Good);
    getAddressSpaceLink()->setFF11_status(status,OpcUa_Good);
    getAddressSpaceLink()->setFF11_txdisable(txdisable,OpcUa_Good);
    getAddressSpaceLink()->setFF11_cdrenable(cdrenable,OpcUa_Good);
    getAddressSpaceLink()->setFF11_cdrrate(cdrrate,OpcUa_Good);
    getAddressSpaceLink()->setFF11_cdrlol(cdrlol,OpcUa_Good);
    getAddressSpaceLink()->setFF11_los(los,OpcUa_Good);
    getAddressSpaceLink()->setFF11_txfault(txfault,OpcUa_Good);
    getAddressSpaceLink()->setFF11_tempfault(tempfault,OpcUa_Good);
    getAddressSpaceLink()->setFF11_voltfault(voltfault,OpcUa_Good);
    getAddressSpaceLink()->setFF11_powerfault(powerfault,OpcUa_Good);
    getAddressSpaceLink()->setFF11_uptime(uptime,OpcUa_Good);
    getAddressSpaceLink()->setFF11_tempC(tempC,OpcUa_Good);
    getAddressSpaceLink()->setFF11_rxpower_0(rxpower_0,OpcUa_Good);
    getAddressSpaceLink()->setFF11_rxpower_1(rxpower_1,OpcUa_Good);
    getAddressSpaceLink()->setFF11_rxpower_2(rxpower_2,OpcUa_Good);
    getAddressSpaceLink()->setFF11_rxpower_3(rxpower_3,OpcUa_Good);
    getAddressSpaceLink()->setFF11_id(id,OpcUa_Good);
    getAddressSpaceLink()->setFF11_model(model,OpcUa_Good);
    getAddressSpaceLink()->setFF11_serial(serial,OpcUa_Good);
    getAddressSpaceLink()->setFF11_fwversion(fwversion,OpcUa_Good);
  }
  else if (ffnumber == 12) {
    getAddressSpaceLink()->setFF12_present(present,OpcUa_Good);
    getAddressSpaceLink()->setFF12_status(status,OpcUa_Good);
    getAddressSpaceLink()->setFF12_txdisable(txdisable,OpcUa_Good);
    getAddressSpaceLink()->setFF12_cdrenable(cdrenable,OpcUa_Good);
    getAddressSpaceLink()->setFF12_cdrrate(cdrrate,OpcUa_Good);
    getAddressSpaceLink()->setFF12_cdrlol(cdrlol,OpcUa_Good);
    getAddressSpaceLink()->setFF12_los(los,OpcUa_Good);
    getAddressSpaceLink()->setFF12_txfault(txfault,OpcUa_Good);
    getAddressSpaceLink()->setFF12_tempfault(tempfault,OpcUa_Good);
    getAddressSpaceLink()->setFF12_voltfault(voltfault,OpcUa_Good);
    getAddressSpaceLink()->setFF12_powerfault(powerfault,OpcUa_Good);
    getAddressSpaceLink()->setFF12_uptime(uptime,OpcUa_Good);
    getAddressSpaceLink()->setFF12_tempC(tempC,OpcUa_Good);
    getAddressSpaceLink()->setFF12_rxpower_0(rxpower_0,OpcUa_Good);
    getAddressSpaceLink()->setFF12_rxpower_1(rxpower_1,OpcUa_Good);
    getAddressSpaceLink()->setFF12_rxpower_2(rxpower_2,OpcUa_Good);
    getAddressSpaceLink()->setFF12_rxpower_3(rxpower_3,OpcUa_Good);
    getAddressSpaceLink()->setFF12_id(id,OpcUa_Good);
    getAddressSpaceLink()->setFF12_model(model,OpcUa_Good);
    getAddressSpaceLink()->setFF12_serial(serial,OpcUa_Good);
    getAddressSpaceLink()->setFF12_fwversion(fwversion,OpcUa_Good);
  }
  else if (ffnumber == 13) {
    getAddressSpaceLink()->setFF13_present(present,OpcUa_Good);
    getAddressSpaceLink()->setFF13_status(status,OpcUa_Good);
    getAddressSpaceLink()->setFF13_txdisable(txdisable,OpcUa_Good);
    getAddressSpaceLink()->setFF13_cdrenable(cdrenable,OpcUa_Good);
    getAddressSpaceLink()->setFF13_cdrrate(cdrrate,OpcUa_Good);
    getAddressSpaceLink()->setFF13_cdrlol(cdrlol,OpcUa_Good);
    getAddressSpaceLink()->setFF13_los(los,OpcUa_Good);
    getAddressSpaceLink()->setFF13_txfault(txfault,OpcUa_Good);
    getAddressSpaceLink()->setFF13_tempfault(tempfault,OpcUa_Good);
    getAddressSpaceLink()->setFF13_voltfault(voltfault,OpcUa_Good);
    getAddressSpaceLink()->setFF13_powerfault(powerfault,OpcUa_Good);
    getAddressSpaceLink()->setFF13_uptime(uptime,OpcUa_Good);
    getAddressSpaceLink()->setFF13_tempC(tempC,OpcUa_Good);
    getAddressSpaceLink()->setFF13_rxpower_0(rxpower_0,OpcUa_Good);
    getAddressSpaceLink()->setFF13_rxpower_1(rxpower_1,OpcUa_Good);
    getAddressSpaceLink()->setFF13_rxpower_2(rxpower_2,OpcUa_Good);
    getAddressSpaceLink()->setFF13_rxpower_3(rxpower_3,OpcUa_Good);
    getAddressSpaceLink()->setFF13_id(id,OpcUa_Good);
    getAddressSpaceLink()->setFF13_model(model,OpcUa_Good);
    getAddressSpaceLink()->setFF13_serial(serial,OpcUa_Good);
    getAddressSpaceLink()->setFF13_fwversion(fwversion,OpcUa_Good);
  }

}

// get fpga sensor data from the JSON structure
void DRegs::extract_fpga(cJSON *sensor_json) {
  cJSON *FPGA_json;
  cJSON *FPGA_up_json, *FPGA_temp_json, *FPGA_vint_json, *FPGA_vaux_json, *FPGA_vbram_json;
  double FPGA_up = -99, FPGA_temp = -99, FPGA_vint = -99, FPGA_vaux = -99, FPGA_vbram = -99;
  char  *FPGA_temp_str, *FPGA_vint_str, *FPGA_vaux_str, *FPGA_vbram_str;

  // first locate the fpga pointer
  if (sensor_json) FPGA_json = cJSON_GetObjectItem(sensor_json, "fpga");

  // now get the actual data
  if (FPGA_json) FPGA_up_json = cJSON_GetObjectItem(FPGA_json, "up");
  if (FPGA_json) FPGA_temp_json = cJSON_GetObjectItem(FPGA_json, "temp");
  if (FPGA_json) FPGA_vint_json = cJSON_GetObjectItem(FPGA_json, "vint");
  if (FPGA_json) FPGA_vaux_json = cJSON_GetObjectItem(FPGA_json, "vaux");
  if (FPGA_json) FPGA_vbram_json = cJSON_GetObjectItem(FPGA_json, "vbram");
  
  if (FPGA_up_json)    FPGA_up = FPGA_up_json->valuedouble; // the uptime is stored as a number
  if (FPGA_temp_json)  FPGA_temp_str = FPGA_temp_json->valuestring;
  if (FPGA_vint_json)  FPGA_vint_str = FPGA_vint_json->valuestring;
  if (FPGA_vaux_json)  FPGA_vaux_str = FPGA_vaux_json->valuestring;
  if (FPGA_vbram_json) FPGA_vbram_str = FPGA_vbram_json->valuestring;

  // these values are stored as strings and need to be converted
  FPGA_temp = std::stod(FPGA_temp_str);
  FPGA_vint = std::stod(FPGA_vint_str);
  FPGA_vaux = std::stod(FPGA_vaux_str);
  FPGA_vbram = std::stod(FPGA_vbram_str);

  // link data to OpcUA
  getAddressSpaceLink()->setFPGA_up(FPGA_up,OpcUa_Good);
  getAddressSpaceLink()->setFPGA_temp(FPGA_temp,OpcUa_Good);
  getAddressSpaceLink()->setFPGA_vint(FPGA_vint,OpcUa_Good);
  getAddressSpaceLink()->setFPGA_vaux(FPGA_vaux,OpcUa_Good);
  getAddressSpaceLink()->setFPGA_vbram(FPGA_vbram,OpcUa_Good);

}

} // Device
