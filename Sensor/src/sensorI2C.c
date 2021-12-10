/*
 * $Id: 
 *-
 *-   Purpose and Methods: Encapsulate the direct I2C reading of the LArC
 *-     sensors.
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created  16-AUG-2017   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/

#include <string.h>
#include <stdio.h>

#include "xtypes.h"
#include "sensorData.h"
#include "sensorI2C.h"
#include "i2cBus.h"


/* ------------------------------------------------------------------------  */
/* -- The following four routines are based on the corresponding IPMC code   */
/* -- and form the local bus master (I2C) version of the code (no IPMC)      */
/* ------------------------------------------------------------------------  */

static u8 iicbuffer[512];

u8* sensorRead(struct sensorI2CAddress *sa, u8 reg, int nbytes) {
  int retcode;

  if( !sa ) return 0;

  /* Extract the bus information */
  int bnum = sa->i2cBusNumber;
  struct i2cBus *bus = geti2cBusInstance(bnum);
  if( !bus ) {
    printf("ERROR:sensorI2C, bad bus number %d\n",bnum);
    seti2cStatus(1);
    return 0;
  }

  int busID = bus->fd;
  if( busID < 0 ) {
    busID = bus->open(bus->name);
    bus->fd = busID;
  }

  /* We need to use repeated starts for this process, or we lose the switch settings */
  if( bus->lock ) bus->lock(busID);

  /* Set up the switch or enable register. */
  if( sa->switchChannelSelector != 0 ) { 
    int isSwitch = ((sa->switchChannel) & 0x80 ) == 0;  /* Highest bit indicates switch or register */
    u8 channel = (sa->switchChannel & 0x7F);
    u8 addr = sa->switchChannelSelector;
    if( isSwitch ) seti2cStatus(bus->muxen(busID,addr,channel));
    else seti2cStatus(bus->regen(busID,addr,channel));
    if( geti2cStatus() < 0 ) {
      printf("ERROR: sensorI2C, I2CMuxEnable returns failure. BUS LOCKED?\n");
      if( bus->unlock ) bus->unlock(busID);
      return 0;
    }
  }

  /* Then do the transaction, first checking to see if an offset is needed
   * for the register to be read.
   */
  if( reg ) {
    iicbuffer[0] = reg;
    retcode = bus->write(busID,sa->deviceAddr,1,iicbuffer);
    seti2cStatus(retcode);
    if( retcode < 0  ) {
      printf("ERROR: sensorAccess, register select returns failure(2). BUS LOCKED?\n");
      if( bus->unlock ) bus->unlock(busID);
      return 0;
    }
  }

  /* and finally the read itself. Unlock before the last transaction */
  seti2cStatus(bus->read(busID,sa->deviceAddr,nbytes,iicbuffer));
  if( bus->unlock ) bus->unlock(busID);
  if( geti2cStatus() >= 0 ) return iicbuffer;
  
  /* Unknown bus/sensor. Just return dummy values */
  iicbuffer[0] = iicbuffer[1] = iicbuffer[2] = iicbuffer[3] = 0xFF;
  return 0; /* Fail */
}

u8* sensorWrite(struct sensorI2CAddress *sa, u8 *val, int nbytes) {
  if( !sa ) return 0;

  /* Extract the bus information */
  int bnum = sa->i2cBusNumber;
  struct i2cBus *bus = geti2cBusInstance(bnum);
  if( !bus ) {
    printf("ERROR:sensorI2C, bad bus number %d\n",bnum);
    seti2cStatus(1);
    return 0;
  }

  int busID = bus->fd;
  if( busID < 0 ) {
    busID = bus->open(bus->name);
    bus->fd = busID;
  }

  /* We need to use repeated starts for this process, or we lose the switch settings */
  if( bus->lock ) bus->lock(busID);

  /* Set up the switch or enable register. */
  if( sa->switchChannelSelector != 0 ) { 
    int isSwitch = ((sa->switchChannel) & 0x80 ) == 0;  /* Highest bit indicates switch or register */
    u8 channel = (sa->switchChannel & 0x7F);
    u8 addr = sa->switchChannelSelector;
    if( isSwitch ) seti2cStatus(bus->muxen(busID,addr,channel));
    else seti2cStatus(bus->regen(busID,addr,channel));
    if( geti2cStatus() == XST_FAILURE ) {
      printf("ERROR: sensorI2C, I2CMuxEnable returns failure. BUS LOCKED?\n");
      if( bus->unlock ) bus->unlock(busID);
      return 0;
    }
  }
  /* Then do the transaction  */
  if( bus->unlock ) bus->unlock(busID);
  seti2cStatus(bus->write(busID,sa->deviceAddr,nbytes,val));
  if( geti2cStatus() >= 0 ) return val;

  return 0; /* Fail */
}
		      
int sensorI2CStatus() { return geti2cStatus(); }
