/*
 * $Id: 
 *-
 *-   Purpose and Methods: Firefly access and control. Also the same status
 *-     as reported by sensorSRTM. The information in the sensor data reading
 *-     is used for i2c addresses, etc.
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created   2-DEC-2020   John D. Hobbs
 *-   Modified  13-OCT-2022  Elliott Cheu
 *-      - turn into a callable function
 *- $Revision: 1.3 $
 *-
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sensorData.h"
#include "sensorI2C.h"
#include "i2cBus.h"
#include "i2cDevUser.h"
#include "cJSON.h"
#include "firefly.h"

/* Each sensor is one physical firefly package. This sets up the access ala the
 * sensor reading code. We want consistent addressing. */
#define NFIREFLY 6 /* 4 TX only + 2 TX/RX */
/*
 * This is used only to provide a block of storage to avoid memory fragmenting.
 * The information in this is accessible via the sensorRecord array below.
 */
static struct sensorReadings {
  struct firefly12 txrx12[NFIREFLY-2];
  struct firefly4 txrx4[2];
} sensorData;


/* Reuse the sensor structure to handle addressing, etc for the firefly modules. Also for getting status. */
static struct sensorRecord sensors[NFIREFLY] = {
  /* These records are the actual Firefly access */
  {{0,FFOutputReg,0x80 | 0, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12},{-1,0},&sensorData.txrx12[0],"FF-TX4"},
  {{0,FFOutputReg,0x80 | 2, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12},{-1,0},&sensorData.txrx12[1],"FF-TX3"},
  {{0,FFOutputReg,0x80 | 4, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12},{-1,0},&sensorData.txrx12[2],"FF-TX2"},
  {{0,FFOutputReg,0x80 | 6, FF25TXONLY, fireflyInit12,fireflyRead12,fireflyFormat12},{-1,0},&sensorData.txrx12[3],"FF-TX1"},
  //{{0,0,0, FF25RXONLY, fireflyInit,fireflyRead,fireflyFormat},{-1,0},&sensorData.txrx[0],"Zynq Diode"},
  //{{0,0,0, FF25RXONLY, fireflyInit,fireflyRead,fireflyFormat},{-1,0},&sensorData.txrx[0],"Zynq Diode"},
  {{0,FFOutputReg,0x80 | 12, FF10TXRX, fireflyInit4,fireflyRead4,fireflyFormat4},{-1,0},&sensorData.txrx4[0],"FF-TXRX12"},
  {{0,FFOutputReg,0x80 | 14, FF10TXRX, fireflyInit4,fireflyRead4,fireflyFormat4},{-1,0},&sensorData.txrx4[1],"FF-TXRX11"}
};

/* Indices into the sensors record above */
static enum ffID {NONE=-2, ALL = -1, FF1=3, FF2=2, FF3=1, FF4=0, FF12=4, FF11=5};

/* Command indices and names */
#define CMD_UNKNOWN -1
#define CMD_ENABLE 0   /* Enable the I2C access to this module, and disable all others. */
#define CMD_DISABLE 1  /* Disable the I2C access to this module. */
#define CMD_STATUS 2   /* Read the module information. */
#define CMD_RESET  3   /* Toggle the module reset line. */
#define CMD_ON     4   /* Turn on the channel. Others are unaffected */
#define CMD_OFF    5   /* Turn off the channel. Others are unaffected */


#ifndef EXTERNAL_I2C
/* This is used when the Zynq is reading the data. This is the standard case. */
static struct i2cBus srtmBuses[] = {
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,0,0,-1,"/dev/i2c-1"}
};
#else
/* This is used when reading data via the BeagleBone. Non-default use. */
static struct i2cBus srtmBuses[] = {
  {I2COpenDevice,I2CCloseDevice,I2CRead,I2CWrite,I2CMuxEnable,I2CRegEnable,0,0,-1,"/dev/i2c-2"}
};
#endif

static void usage() {
  printf("Usage: fireflyControl <device> <command> [<channel>]\n");
  printf("   where <device> is one: of all, fex1, fex2, fex3, fex4, felix1, or felix2\n");
  printf("         <command> is \"enable\", or \"disable\" or \"reset\" or \"status\"\n");
  printf(" or    fireflyControl <device> <command> <chan>\n");
  printf("   where <device> is one: of all, fex1, fex2, fex3, fex4, felix1, or felix2\n");
  printf("         <command> is \"on\", or \"off\"\n");
  printf("         <chan> is an integer in the range 0 - 11 or 'all' for the fex fireflies\n");
  printf("             or is an integer in the range 0 - 3 or 'all' for the felix fireflies\n");
  printf("\n");
  exit(1);
}

/* Get the index into the Firefly I2C address array for the chosen FF */
enum ffID getFFID(char *ffname) {
  /* Make a local copy and convert to lower case */
  char *lffname = malloc(strlen(ffname)+1);
  int i=0;
  for( i=0 ; i<strlen(ffname) ; i++ ) {
    lffname[i] = ffname[i];
    if( lffname[i]>='A' && lffname[i]<='Z' ) lffname[i] = lffname[i]+32;
  }

  if( strcmp(lffname,"all") == 0  ) return ALL; 
  if( strcmp(lffname,"fex1") == 0  ) return FF1;
  if( strcmp(lffname,"fex2") == 0  ) return FF2;
  if( strcmp(lffname,"fex3") == 0  ) return FF3;
  if( strcmp(lffname,"fex4") == 0  ) return FF4;
  if( strcmp(lffname,"felix1") == 0  ) return FF11;
  if( strcmp(lffname,"felix2") == 0  ) return FF12;
  printf("FATAL: Invalid firefly device name '%s'\n",lffname);
  usage();
  return NONE; /* Actually, usage() calls exit, so we'll never get here */
}

/* Convert the command line command name into an index for a switch statement */
static int getCommand(char *token) {
  if( !strcmp(token,"enable") ) return CMD_ENABLE;
  if( !strcmp(token,"disable") ) return CMD_DISABLE;
  if( !strcmp(token,"status") ) return CMD_STATUS;
  if( !strcmp(token,"reset") ) return CMD_RESET;
  if( !strcmp(token,"on") ) return CMD_ON;
  if( !strcmp(token,"off") ) return CMD_OFF;
  return CMD_UNKNOWN;
}

static int isChannelCommand(int cmd) {
  if( cmd == CMD_ON ) return 1;
  if( cmd == CMD_OFF ) return 1;
  return 0;
}

static int getNChan(enum ffID anff) {
  switch( anff ) {
  case FF1:
  case FF2:
  case FF3:
  case FF4:
    return 12;
  case FF11:
  case FF12:
    return 4;
  default:
    return -1;
  }
  return -1;
}

static int getChannel(enum ffID theff, const char *snum) {
  /* If all channels are requested, return the number, not the index */
  if( !strcmp(snum,"all") ) return getNChan(theff);

  /* Otherwise, convert to an integer and return that if it's within the allowed range */
  int ch = atoi(snum);
  if( ch<0 || ch>=getNChan(theff) ) return -1;
  return ch;
}

static int init=0, hasIPMC=0; /* Always leave hasIPMC=0 for HL-LHC */

int sensorInit() {
  if( init ) return 0;
  //printf("In %s, %s\n",__FILE__,__FUNCTION__);
  i2cBusRegister(1,srtmBuses);
  sensorRegister(NFIREFLY,sensors,hasIPMC);
  init = 1;
  return 0;
}

int fcIsPresent(enum ffID aFirefly) { 
  /* Just get the address record and call the utility routine */
  struct sensorI2CAddress *ffsa = &(sensors[aFirefly].saI2C);
  return fireflyIsPresent(ffsa);
}

int fcEnable(enum ffID aFirefly) {
  /* Just get the address record and call the utility routine */
  struct sensorI2CAddress *ffsa = &(sensors[aFirefly].saI2C);
  return fireflyEnableI2C(ffsa);
}

int fcDisable(enum ffID aFirefly) {
  /* Just get the address record and call the utility routine */
  struct sensorI2CAddress *ffsa = &(sensors[aFirefly].saI2C);
  return fireflyDisableI2C(ffsa);
}

int fcReset(enum ffID aFirefly) {
  /* Just get the address record and call the utility routine */
  struct sensorI2CAddress *ffsa = &(sensors[aFirefly].saI2C);
  return fireflyReset(ffsa);
}

#define TXDIS4 86
#define TXDIS12 52

int fcChanEnable(enum ffID aFirefly, int ichan) {
  fcEnable(aFirefly);
  printf("In %s, %s with ffID = %d, ichan =%d\n",__FILE__,__FUNCTION__,aFirefly,ichan);
  struct sensorI2CAddress *ffsa = &(sensors[aFirefly].saI2C);
  /* The enable register format depends on which version it is. */
  if( ffsa->initfcn == fireflyInit4 ) { /* 4 channel Tx/Rx module */ 
	  u8 *data = sensorRead(ffsa,TXDIS4,1);
	  u8 oldval = data[0];
	  u8 ibit = 1<<ichan;
	  u8 mask = ~ibit;
	  u8 newval[2] = {TXDIS4, oldval & mask};  /* clear the disable bit for this channel */
	  sensorWrite(ffsa,newval,2);
	  data = sensorRead(ffsa,TXDIS4,1);
	  printf("    initial tx disable register = 0x%02x; after enabling channel %d, = 0x%02x\n",oldval,ichan,data[0]);
  }
  else { /* 12 channel Tx module */
	  u8 *data = sensorRead(ffsa,TXDIS12,2);
	  u16 oldval = (( data[0] << 8 ) & 0xFF00 ) | data[1];
	  u16 ibit = 1<<ichan;
	  u16 mask = ~ibit;
    u16 mval = oldval & mask;
	  u8 newval[3] = {TXDIS12, ((mval>>8) & 0xFF) , mval & 0xFF};  /* clear the disable bit for this channel */
	  sensorWrite(ffsa,newval,3);
	  data = sensorRead(ffsa,TXDIS12,2);
    mval =  (( data[0] << 8 ) & 0xFF00 ) | data[1];
	  printf("    initial tx disable register = 0x%03x; after enabling channel %d, = 0x%03x\n",oldval,ichan,mval);
  }
  fcDisable(aFirefly);
  return 0;
}

int fcChanDisable(enum ffID aFirefly, int ichan) {
  fcEnable(aFirefly);
  printf("In %s, %s with ffID = %d, ichan =%d\n",__FILE__,__FUNCTION__,aFirefly,ichan);
  struct sensorI2CAddress *ffsa = &(sensors[aFirefly].saI2C);
  /* The enable register format depends on which version it is. */
  if( ffsa->initfcn == fireflyInit4 ) { /* 4 channel Tx/Rx module */ 
	  u8 *data = sensorRead(ffsa,TXDIS4,1);
	  u8 oldval = data[0];
	  u8 ibit = 1<<ichan;
	  u8 newval[2] = {TXDIS4, oldval | ibit}; /* Set the register address and then the disable bit for this channel */
	  sensorWrite(ffsa,newval,2);
	  data = sensorRead(ffsa,TXDIS4,1);
	  printf("    initial tx disable register = 0x%02x; after disabling channel %d, = 0x%02x\n",oldval,ichan,data[0]);
  }
  else { /* 12 channel Tx module */
	  u8 *data = sensorRead(ffsa,TXDIS12,2);
	  u16 oldval = (( data[0] << 8 ) & 0xFF00 ) | data[1];
	  u16 ibit = 1<<ichan;
    u16 mval = oldval | ibit;
	  u8 newval[3] = {TXDIS12, ((mval>>8) & 0xFF) , mval & 0xFF};  /* clear the disable bit for this channel */
	  sensorWrite(ffsa,newval,3);
	  data = sensorRead(ffsa,TXDIS12,2);
    mval =  (( data[0] << 8 ) & 0xFF00 ) | data[1];
	  printf("    initial tx disable register = 0x%03x; after disabling channel %d, = 0x%03x\n",oldval,ichan,mval);
  }
  fcDisable(aFirefly);
  return 0;
}

/* This uses the usual sensor-based mechanism to get the status info. */
int getStatus(enum ffID aFirefly) {
  if( !fcIsPresent(aFirefly) ) return 1;

   /* Read the data directly over the I2C bus */
  struct sensorI2CAddress *saI2C = &(sensors[aFirefly].saI2C);
  void *sdI2C = sensors[aFirefly].valueBuffer;
  if( saI2C->readfcn ) saI2C->readfcn(saI2C,sdI2C);

  /* and now try a formatted dump */
  cJSON *top = cJSON_CreateObject();
  if( saI2C->fmtfcn ) saI2C->fmtfcn(&sensors[aFirefly],top);
  printf("%s\n",cJSON_Print(top));
  cJSON_Delete(top);
  return 0;
}

int executeChannelCommand(enum ffID theFF, int ichan, int icmd) {
  if( !fcIsPresent(theFF) ) {
	printf("Firefly %d not present. Exiting\n",theFF);
	return -2;
  }
	
  switch( icmd ) {
  case CMD_ON:
    return fcChanEnable(theFF,ichan);
  case CMD_OFF:
    return fcChanDisable(theFF,ichan);
  }
  return -1;
}

void executeAllChannelCommand(enum ffID theFF, int icmd) {
  if( !fcIsPresent(theFF) ) {
	printf("Firefly %d not present. Skipping\n",theFF);
	return;
  }

  int ic=0;
  for( ic=0 ; ic<getNChan(theFF) ; ic++ ) {
    executeChannelCommand(theFF,ic,icmd);
  }    
}

int executeCommand(enum ffID theFF, int icmd) {
	
  if( !fcIsPresent(theFF) ) {
	printf("Firefly %d not present. Skipping\n",theFF);
	return -2;
  }
  
  switch( icmd ) {
  case CMD_ENABLE:
    return fcEnable(theFF);
  case CMD_DISABLE:
    return fcDisable(theFF);
  case CMD_RESET:
    return fcReset(theFF);
  case CMD_STATUS:
    return getStatus(theFF);
  }
  return -1;
}

void executeAllCommand(int icmd) {
  int i=0;
  for( i=0 ; i<NFIREFLY ; i++ ) {
    executeCommand(i,icmd);
  }    
}

// ECC - command to turn on FF11
void enableFF11() {
  fcEnable(FF11);
}
// ECC - command to turn off FF11
void disableFF11() {
  fcDisable(FF11);
}

#ifdef STANDALONE
int main(int argc, char **argv) {
  if( argc != 3 && argc !=4 ) usage();

  /* Parse the command line */
  enum ffID ffsel = getFFID(argv[1]);
  if( ffsel == NONE )  usage();
  if( ffsel<ALL && ffsel>FF12 ) { printf("FATAL: Invalid module = '%s'\n",argv[1]); usage(); }
  int icmd = getCommand(argv[2]);
  if( icmd == CMD_UNKNOWN ) { printf("FATAL: Invalid command: %s\n",argv[2]); usage(); }

  /* Is there an extra argument needed? The channel... */
  if( !isChannelCommand(icmd) ) { /* These are per module commands */
    if( argc != 3 ) usage();
    sensorInit();
    if( ffsel == ALL ) executeAllCommand(icmd);
    else executeCommand(ffsel,icmd);
  }
  else { /* These are per channel commands */
    if( argc != 4 ) usage();
    /* Get the channel number */
    int ffchan = getChannel(ffsel,argv[3]);
    if( ffchan<0 ) usage();
    /* and now execute the command */
    sensorInit();
    if( ffchan == getNChan(ffsel) ) executeAllChannelCommand(ffsel,icmd);
    else executeChannelCommand(ffsel,ffchan,icmd);
  }

  return 0;
}
#endif

  
