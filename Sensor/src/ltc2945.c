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
#define _LTC2945_C_
#include <string.h>
#include <stdio.h>

#include "ltc2945.h"
#include "sensorI2C.h"

/* These are the calibration values for the HL-LHC system(s). This would be soooo much easier in C++ */
const float* c_zynq_diode() { static const float cvals[] = {0.22,345.0}; return cvals; }
const float* c_ltm4628() { static const float cvals[] = {0.22,242.0}; return cvals; }  /* Also for ltm4616 */
const float* c_ltm4630() { static const float cvals[] = {0.25,317.0}; return cvals; }  /* Also for ltm4650 */

void ltc2945Init(struct sensorI2CAddress *sa) {
  u8 buff[3];
  buff[0] = 0x00;  // Register Address - Control
  buff[1] = 0x05;  // Initialize Chip
  sensorWrite(sa,buff,2);
}

void ltc2945Read(struct sensorI2CAddress *sa, void *valueBuffer) {
  struct ltc2945 *dest = (struct ltc2945*)(valueBuffer);

#ifndef CAN_DO_RESTART

  u8 *data = sensorRead(sa,0x0,48);
  if( !data ) { memset(dest,0,sizeof(struct ltc2945)); dest->noUpd=1; return; }
  dest->noUpd=0;
  
  dest->rawVolts  = (data[0x1E] << 4); // buff[0] = MSB of Voltage Sense Reg - Bits 11 .. 4
  dest->rawVolts |= (data[0x1F] >> 4); // buff[1] = LSB - OR in LSB
  dest->voltage = (102.4 / 4096.0) * (float)dest->rawVolts;

  dest->rawAmps  = (data[0x14] << 4); // ltc2945buff[0] = MSB of Current Sense Reg - Bits 11 .. 4
  dest->rawAmps |= (data[0x15] >> 4); // ltc2945buff[1] = LSB - OR in LSB
  dest->current = (0.1024 / 4096.0) * (float)(dest->rawAmps);
  dest->current = dest->current / 0.001;

  /* Optional temperature */
  if( !data[0x28] && !data[0x29]) return;
  dest->rawADIN  = (data[0x28] << 4) & 0xFF0; // Some ltc's also have temperature monitoring.
  dest->rawADIN |= ((data[0x29] >> 4) & 0xF);

  if( sa->params ) {
    float vin = dest->rawADIN;
    float *calib = (float *)(sa->params());
    //printf("params = %p, calib = %p\n",sa->params,calib);
    float slope = calib[0];       // FPGA = 0.22, ltm4628/16 = 0.22, ltm4628/50 = 0.25
    float offset = calib[1];     // FPGA = 345.,            = 242.,              327
    float temperature = offset - vin*slope;
    //printf("data = 0x%02x/0x%02x raw = %d, vin = %f\n",data[0x28],data[0x29],dest->rawADIN,temperature);
    dest->temperature = temperature;
  }
  else { /* Use the Phase I calibration */
    float vin = (2.048/4096.) * (float)(dest->rawADIN);
    dest->temperature = vin/198e-6 - 273.15;  /* See LTM4650 for the calibration constant */
    dest->temperature = vin;
  }
  
#else  

  /* Voltage */
  u8 *data = sensorRead(sa,0x1E,2);
  if( !data ) { memset(dest,0,sizeof(struct ltc2945)); dest->noUpd=1; return; }
  
  dest->rawVolts  = (data[0] << 4); // buff[0] = MSB of Voltage Sense Reg - Bits 11 .. 4
  dest->rawVolts |= (data[1] >> 4); // buff[1] = LSB - OR in LSB
  dest->voltage = (102.4 / 4096.0) * (float)dest->rawVolts;

  /* Current */
  data = sensorRead(sa,0x14,2);
  if( !data ) { memset(dest,0,sizeof(struct ltc2945)); dest->noUpd=1; return; }

  dest->rawAmps  = (data[0] << 4);   // ltc2945buff[0] = MSB of Current Sense Reg - Bits 11 .. 4
  dest->rawAmps |= (data[0x1] >> 4); // ltc2945buff[1] = LSB - OR in LSB
  dest->current = (0.1024 / 4096.0) * (float)(dest->rawAmps);
  dest->current = dest->current / 0.001;

  if( sa->params ) {
    float vin = dest->rawADIN;
    float *calib = (float *)(sa->params());
    float slope = calib[0];       // FPGA = 0.22, ltm4628/16 = 0.22, ltm4628/50 = 0.25
    float offset = calib[1];     // FPGA = 345.,            = 242.,              327
    float temperature = offset - vin*slope;
    //printf("data = 0x%02x/0x%02x raw = %d, vin = %f\n",data[0x28],data[0x29],dest->rawADIN,temperature);
    dest->temperature = temperature;
  }
  else { /* Use the Phase I calibration */
    float vin = (2.048/4096.) * (float)(dest->rawADIN);
    dest->temperature = vin/198e-6 - 273.15;  /* See LTM4650 for the calibration constant */
    dest->temperature = vin;
  }    
#endif
  
  //printf("ltc2945Read: voltage = %.2f, current = %.2f, temperature = %.2f\n",dest->voltage,dest->current,dest->temperature);
}


void ltc2945Format(struct sensorRecord *ltcSensor, struct cJSON *parent) {
  struct ltc2945 *tmp = (struct ltc2945*)(ltcSensor->valueBuffer);
  if( !tmp ) return;
  cJSON *viarr = cJSON_CreateArray();
  cJSON_AddItemToArray(viarr,cJSON_CreateString(ltcSensor->name));
  cJSON_AddItemToArray(viarr,cJSON_CreateNumber(tmp->noUpd) );
  cJSON_AddItemToArray(viarr,cJSON_CreateNumber(tmp->voltage) );
  cJSON_AddItemToArray(viarr,cJSON_CreateNumber(tmp->current) );
  cJSON_AddItemToArray(viarr,cJSON_CreateNumber(tmp->temperature) );
  cJSON_AddItemToObject(parent,"ltc2945",viarr);
}

