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
*/

#include <string.h>
#include <stdio.h>

#include "xtypes.h"
#include "i2cBus.h"

/* This is the mandatory i2cBus array registered by code for specific boards
 * typically via the SensorInit routine.
 */
static struct i2cBus *i2cBusList=0;
static int ni2cBus=0;
static int i2cBusErrCode=0;

void i2cBusRegister(int nbus, struct i2cBus *busarray) {
  i2cBusList = busarray;
  ni2cBus = nbus;
}

struct i2cBus* geti2cBusInstance(int ibus) {
  if( ibus<0 || ibus>=ni2cBus ) return 0;
  return &i2cBusList[ibus];
}

int geti2cBusCount() { return ni2cBus; }

		      
int geti2cStatus() { return i2cBusErrCode; }
void seti2cStatus(int status) { i2cBusErrCode = status; }

static int debug=0;
void seti2cDebugLevel(int dbg) { debug = dbg; }
int  geti2cDebugLevel() { return debug; }
