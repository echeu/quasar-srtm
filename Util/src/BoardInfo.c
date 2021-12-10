/*
 * hwvers.c
 *
 *  Created on: Aug. 16, 2016
 *      Author: hobbs
 *
 *  Purpose: Wrapper around the board hardware ID (eFuse) information. See "eFuse definition.gsheet" for hw ID definitions.
 *             Definitions will also be propagated to the carrier document.
 */

#include "BoardInfo.h"

#ifdef __linux__

#include "xtypes.h"
#include "uio_setup.h"

static void *biDevice=0;
static int initDone=0;

#else

#include "xil_types.h"
#include "xparameters.h"

#endif

u8 hasHWInfo() {
#ifdef __linux__
  if( !initDone ) {
    biDevice = uio_setup("axiboardinfo");
    initDone = 1;
  }
  return biDevice != 0;
#else
#ifdef XPAR_AXI_BOARDINFO_0_BASEADDR
	return 1;
#else
	return 0;
#endif
#endif
}

u8 hwTypeOTC() { /* See "eFuse definition.gsheet" */
	u8 hwtype = getHWType();
	if( hwtype>=3 && hwtype<=4 ) return 1;
	return 0;
}

u8 hwTypeATCA() {
	u8 hwtype = getHWType();
	if( hwtype>=1 && hwtype<=2 ) return 1;
	return 0;
}

u8 hwTypeSRTM() {
	u8 hwtype = getHWType();
	if( hwtype>=5 && hwtype<=7 ) return 1;
	return 0;
}

u8 hwTypeSRTMTester() {
	u8 hwtype = getHWType();
	if( hwtype>=8 && hwtype<=9 ) return 1;
	return 0;
}

u32 getHWEFuse() {
#ifdef __linux__
  if( !hasHWInfo() ) return 0;
  return *((u32*)biDevice);
#else
#ifdef XPAR_AXI_BOARDINFO_0_BASEADDR
	return *((u32*)(XPAR_AXI_BOARDINFO_0_BASEADDR));
#else
	return 0;
#endif
#endif
}

u8 getHWType() {
	return ( getHWEFuse() >> 24 ) & 0xFF;  /* Automatically returns 0 if not available */
}

u8 getHWRev() {
#ifdef XPAR_AXI_BOARDINFO_0_BASEADDR
	return ( getHWEFuse() >> 16 ) & 0xF;
#else
	return 0;
#endif
}

u8 getHWVer() {
	return ( getHWEFuse() >> 20 ) & 0xF;
}

u8 getHWVerRev() {
	return ( getHWEFuse() >> 16 ) & 0xFF;
}

u8 getHWSerialNumber() {
	return ( getHWEFuse() >> 8 ) & 0xFF;
}

const char* getBoardTypeName() {
  /* Is there a memory leak here or not? */
  if( hasHWInfo() ) {
    if( hwTypeOTC() ) return "OTC";
    else if( hwTypeATCA() ) return "ATCA";
    else if( hwTypeSRTM() ) return "SRTM";
    else if( hwTypeSRTMTester() )  return "SRTMTester";
  }
  else {
#ifdef ZCU111
    return "ZCU111";
#else
    return "UNKNOWN-NI";
#endif
  }
  return "UNKNOWN";
}
