/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created  29-NOV-2020   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/
#include <stdio.h>
#include <string.h>

#include "firefly.h"
#include "cJSON.h"
#include "sensorData.h"
#include "sensorI2C.h"
#include "hexdump.h"

static char* formatFwVers(u8 vers, u8 minor, u8 rev) {
  static char buffer[128];
  sprintf(buffer,"%d.%d.%d",vers,minor,rev);
  return buffer;
}

static int getBit(struct sensorI2CAddress *ffsa, int bitnum) {
  /* Construct a sensor address for the status register. This is just so that we */
  /* can reuse code. The address of the status reg is one less than the control.  */
  struct sensorI2CAddress crAddr = {ffsa->i2cBusNumber,0,0,ffsa->switchChannelSelector-1,0,0,0,0};
  u8 *data = sensorRead(&crAddr,0x0,2);
  if( !data ) return 0;

  /* Repack the data as a 16 bit register and mask to select only one presence line. */
  int value = (data[1]<<8) | ( data[0] & 0xFF ) ;
  int mask = (1 << bitnum); /* Mask all but the requested bit */
  int bitstate = value & mask;
  //printf("getBit: data: 0x%02x 0x%02x, value = 0x%08x, mask = 0x%08x, bitstate = 0x%08x\n",data[0],data[1],value,mask,bitstate);
  return bitstate;
}

static int setBit(struct sensorI2CAddress *ffsa, int bitnum) {
  struct sensorI2CAddress crAddr = {ffsa->i2cBusNumber,0,0,ffsa->switchChannelSelector,0,0,0,0};
  /* Construct a sensor address for the control register. This is just so that we */
  /* can reuse code. The address of the status reg is one less than the control.  */
  u8 *data = sensorRead(&crAddr,0x0,2);
  if( !data ) {
    printf("In %s / %s, read failed\n",__FILE__,__FUNCTION__);
    return -1;
  }
  //printf("In %s / %s, read OK\n",__FILE__,__FUNCTION__);

  int value = (data[1]<<8) | ( data[0] & 0xFF ) ;
  int mask = (1 << bitnum);
  int bitstate = value | mask;
  u8 buffer[4];
  buffer[0] = bitstate & 0xFF;
  buffer[1] = (bitstate >> 8 ) & 0xFF;
  //printf("In %s / %s: raw = 0x%02x,0x%02x, value = 0x%04x, mask =0x%04x, bitstate = 0x%04x \n",__FILE__,__FUNCTION__,
  //       data[0],data[1],value,mask,bitstate);
  u8 *retcode = sensorWrite(&crAddr,buffer,2);
  return retcode != 0;
}

static int clrBit(struct sensorI2CAddress *ffsa, int bitnum) {
  struct sensorI2CAddress crAddr = {ffsa->i2cBusNumber,0,0,ffsa->switchChannelSelector,0,0,0,0};
  /* Construct a sensor address for the control register. This is just so that we */
  /* can reuse code. The address of the status reg is one less than the control.  */
  u8 *data = sensorRead(&crAddr,0x0,2);
  if( !data ) {
    printf("In %s / %s, read failed\n",__FILE__,__FUNCTION__);
    return -1;
  }
  //printf("In %s / %s, read OK\n",__FILE__,__FUNCTION__);

  int value = (data[1]<<8) | ( data[0] & 0xFF ) ;
  int mask = ~(1 << bitnum);
  int bitstate = value & mask;
  u8 buffer[4];
  buffer[0] = bitstate & 0xFF;
  buffer[1] = (bitstate >> 8 ) & 0xFF;
  //printf("In %s / %s: raw = 0x%02x,0x%02x, value = 0x%04x, mask =0x%04x, bitstate = 0x%04x \n",__FILE__,__FUNCTION__,
  //       data[0],data[1],value,mask,bitstate);
  u8 *retcode = sensorWrite(&crAddr,buffer,2);
  return retcode != 0;
}

/* ------ Control lines common to all modules -------- */
/* -- The sensor I2C address is the struct for the full FF module, -- */
/* -- and the information for the associated control/status i2c    -- */
/* -- modules are inferred from the FF information. Just for ease. -- */
/* Return: non-zero if the module is present */
int fireflyIsPresent(struct sensorI2CAddress *ffsa) {
  if( !ffsa ) return 0;
  int bitstate = getBit(ffsa, ffsa->switchChannel & 0x7F);
  if( bitstate < 0 ) return 0; /* error */
  return !bitstate; /* present_n */
}

/* Return non-zero if the module interrupt is asserted */
int fireflyIsInInterrupt(struct sensorI2CAddress *ffsa) {
  if( !ffsa ) return 0;
  int bitstate = getBit(ffsa,(ffsa->switchChannel & 0x7F)+1);
  if( bitstate < 0 ) return 0; /* error */
  return !bitstate; /* present_n */
}

/* Toggle the firefly reset line */
int fireflyReset(struct sensorI2CAddress *ffsa) {
  if( !ffsa ) return 0;
  int bitnum = (ffsa->switchChannel & 0x7F) + 1;
  clrBit(ffsa,bitnum);
  return setBit(ffsa,bitnum);
}

/* Enable a specific firefly (only 1 at a time) for I2C access */
int fireflyEnableI2C(struct sensorI2CAddress *ffsa) {
  if( !ffsa ) return 0;
  return clrBit(ffsa,ffsa->switchChannel & 0x7F);
}

/* Disable I2C access the Fireflies */
int fireflyDisableI2C(struct sensorI2CAddress *ffsa) {
  if( !ffsa ) return 0;
  return setBit(ffsa,ffsa->switchChannel & 0x7F);
}


/* ------------------------------------------------------------------------------------ */
/* ---------------------------- 4 channel modules ------------------------------------- */
/* ------------------------------------------------------------------------------------ */
void fireflyInit4(struct sensorI2CAddress *sa) {}

/* This assumes the (SRTM necessary) external enable register has been set. */
void fireflyRead4(struct sensorI2CAddress *sa, void *valueBuffer) {

  struct firefly4 *record = (struct firefly4*)valueBuffer;
  memset(record,0,sizeof(struct firefly4));
  
  if( !fireflyIsPresent(sa) ) return;

  /* Make sure the read starts at address = 0 */
  u8 *data=0;
  u8 buffer[]={0x0};
  data = sensorWrite(sa,buffer,1);
  if( !data ) { printf("Error setting starting address for firefly first block read\n"); return; }

  /* And read the first part */
#define nbytes 210
  u8 rdbuf[nbytes];
  data = sensorRead(sa,0x0,128);
  if( !data ) { printf("Error reading 4 channel firefly first block\n"); return; }
  memcpy(rdbuf,data,128);
  //hexdump(data,nbytes);

  /* Now update the read starting address */
  buffer[0] = 0x80;
  data = sensorWrite(sa,buffer,1);
  if( !data ) { printf("Error setting starting address for firefly second block read\n"); return; }

  /* and read the second part */
  data = sensorRead(sa,0x0,nbytes-128);
  if( !data ) { printf("Error reading 4 channel firefly second block\n"); return; }
  memcpy(&rdbuf[128],data,nbytes-128);
  data = rdbuf;
  //hexdump(data,nbytes);

  record->present = 1;
  record->id = data[0];
  record->status = data[2];
  record->txDisable = data[86];
  record->cdrEnable = data[98];
  record->cdrRate = data[99];
  record->los = data[3];
  record->txfault = data[4];
  record->cdrlol = data[5];
  record->tempfault = data[6];
  record->voltfault = data[7];
  record->powerfault = data[10] << 8 | data[9];
  record->uptime = data[19] << 8 | data[20];
  record->temperatureRaw = data[22];
  memcpy(&(record->rxPowerRaw),&data[34],8);
  memcpy( &(record->part),&data[168],13);
  record->part[13] = 0;
  memcpy( &(record->serial),&data[196],10);
  record->part[10] = 0;
  record->fwVers = data[69];
  record->fwMinor = data[70];
  record->fwRev = data[71];
  
  /* Now convert the RX power to microW */
  int ich=0;
  for( ich=0 ; ich<4 ; ich++ ) record->rxPower[ich] = ((float)record->rxPowerRaw[ich])/10.0;
}

void fireflyFormat4(struct sensorRecord *sr, struct cJSON *parent) {
  struct firefly4 *data = (struct firefly4*)(sr->valueBuffer);
  cJSON *fftop = cJSON_CreateObject();
  cJSON_AddItemToObject(fftop,"name",cJSON_CreateString(sr->name));
  if( !data->present ) {
    cJSON_AddItemToObject(fftop,"present",cJSON_CreateNumber(0));
    cJSON_AddItemToObject(parent,"firefly",fftop);
    return;
  }  
  cJSON_AddItemToObject(fftop,"present",cJSON_CreateNumber(1));
  cJSON_AddItemToObject(fftop,"status",cJSON_CreateNumber(data->status));
  cJSON_AddItemToObject(fftop,"txdisable",cJSON_CreateNumber(data->txDisable));
  cJSON_AddItemToObject(fftop,"cdrenable",cJSON_CreateNumber(data->cdrEnable));
  cJSON_AddItemToObject(fftop,"cdrrate",cJSON_CreateNumber(data->cdrRate));
  cJSON_AddItemToObject(fftop,"cdrlol",cJSON_CreateNumber(data->cdrlol));
  cJSON_AddItemToObject(fftop,"los",cJSON_CreateNumber(data->los));
  cJSON_AddItemToObject(fftop,"txfault",cJSON_CreateNumber(data->txfault));
  cJSON_AddItemToObject(fftop,"tempfault",cJSON_CreateNumber(data->tempfault));
  cJSON_AddItemToObject(fftop,"voltfault",cJSON_CreateNumber(data->voltfault));
  cJSON_AddItemToObject(fftop,"powerfault",cJSON_CreateNumber(data->powerfault));
  cJSON_AddItemToObject(fftop,"uptime",cJSON_CreateNumber(data->uptime));
  cJSON_AddItemToObject(fftop,"tempC",cJSON_CreateNumber(data->temperatureRaw));
  cJSON *rxpower = cJSON_CreateArray();
  cJSON_AddItemToArray(rxpower,cJSON_CreateNumber(data->rxPower[0]));
  cJSON_AddItemToArray(rxpower,cJSON_CreateNumber(data->rxPower[1]));
  cJSON_AddItemToArray(rxpower,cJSON_CreateNumber(data->rxPower[2]));
  cJSON_AddItemToArray(rxpower,cJSON_CreateNumber(data->rxPower[3]));
  cJSON_AddItemToObject(fftop,"rxpower",rxpower);
  cJSON_AddItemToObject(fftop,"id",cJSON_CreateNumber(data->id));
  cJSON_AddItemToObject(fftop,"model",cJSON_CreateString(data->part));
  cJSON_AddItemToObject(fftop,"serial",cJSON_CreateString(data->serial));
  cJSON_AddItemToObject(fftop,"fwversion",cJSON_CreateString(formatFwVers(data->fwVers,data->fwMinor,data->fwRev)));
  cJSON_AddItemToObject(parent,"firefly",fftop);
}

/* ------------------------------------------------------------------------------------ */
/* --- 12 channel modules --- */
/* ------------------------------------------------------------------------------------ */
void fireflyInit12(struct sensorI2CAddress *sa) {}

/* This assumes the (SRTM necessary) external enable register has been set. */
void fireflyRead12(struct sensorI2CAddress *sa, void *valueBuffer) {

  struct firefly12 *record = (struct firefly12*)valueBuffer;
  memset(record,0,sizeof(struct firefly12));
  
  if( !fireflyIsPresent(sa) ) return;

  /* Make sure the read starts at address = 0 */
  u8 *data=0;
  u8 buffer[]={0x0};
  data = sensorWrite(sa,buffer,1);
  if( !data ) { printf("Error setting starting address for firefly first block read\n"); return; }

  /* and read the first part */
  u8 rdbuf[nbytes];
  data = sensorRead(sa,0x0,128);
  if( !data ) { printf("Error reading 4 channel firefly first block\n"); return; }
  memcpy(rdbuf,data,128);
  //hexdump(data,nbytes);

  /* Now update the read starting address */
  buffer[0] = 0x80;
  data = sensorWrite(sa,buffer,1);
  if( !data ) { printf("Error setting starting address for firefly second block read\n"); return; }

  /* and the second part */
  data = sensorRead(sa,0x0,nbytes-128);
  if( !data ) { printf("Error reading 4 channel firefly second block\n"); return; }
  memcpy(&rdbuf[128],data,nbytes-128);
  data = rdbuf;
  //hexdump(data,nbytes);

  record->present = 1;
  record->status = data[2];
  record->tempfault = data[17];
  record->voltfault = data[18];
  record->cdrEnable = data[74] << 8 | data[75];
  record->cdrlol = data[20] << 8 | data[21];
  record->uptime = data[38] << 8 | data[39];
  record->temperatureRaw = data[22];
  memcpy( &(record->fwVers),&data[111],4);
  memcpy( &(record->vendor),&data[152],10);
  memcpy( &(record->part),&data[171],15);
  memcpy( &(record->serial),&data[189],10);

}

void fireflyFormat12(struct sensorRecord *sr, struct cJSON *parent) {
  struct firefly12 *data = (struct firefly12*)(sr->valueBuffer);
  cJSON *fftop = cJSON_CreateObject();
  cJSON_AddItemToObject(fftop,"name",cJSON_CreateString(sr->name));
    if( !data->present ) {
    cJSON_AddItemToObject(fftop,"present",cJSON_CreateNumber(0));
    cJSON_AddItemToObject(parent,"firefly",fftop);
    return;
  }  
  cJSON_AddItemToObject(fftop,"present",cJSON_CreateNumber(1));
  cJSON_AddItemToObject(fftop,"status",cJSON_CreateNumber(data->status));
  cJSON_AddItemToObject(fftop,"chandisable",cJSON_CreateNumber(data->channelDisable));
  cJSON_AddItemToObject(fftop,"outdisable",cJSON_CreateNumber(data->outputDisable));
  cJSON_AddItemToObject(fftop,"cdrenable",cJSON_CreateNumber(data->cdrEnable));
  cJSON_AddItemToObject(fftop,"cdrlol",cJSON_CreateNumber(data->cdrlol));
  cJSON_AddItemToObject(fftop,"tempfault",cJSON_CreateNumber(data->tempfault));
  cJSON_AddItemToObject(fftop,"voltfault",cJSON_CreateNumber(data->voltfault));
  cJSON_AddItemToObject(fftop,"uptime",cJSON_CreateNumber(data->uptime));
  cJSON_AddItemToObject(fftop,"tempC",cJSON_CreateNumber(data->temperatureRaw));
  cJSON_AddItemToObject(fftop,"vendor",cJSON_CreateString(data->vendor));
  cJSON_AddItemToObject(fftop,"model",cJSON_CreateString(data->part));
  cJSON_AddItemToObject(fftop,"serial",cJSON_CreateString(data->serial));
  cJSON_AddItemToObject(fftop,"fwversion",cJSON_CreateNumber(data->fwVers));
  cJSON_AddItemToObject(parent,"firefly",fftop);
}

