/*
 * BoardInfo.h
 *
 *  Created on: Aug. 16, 2016
 *      Author: hobbs
 *
 *  Purpose: Wrapper around the board hardware ID (eFuse) information
 */
#ifndef BOARDINFO_H
#define BOARDINFO_H

#ifdef __linux__
#include "xtypes.h"
#else
#include "xil_types.h"
#endif

u8 hasHWInfo();

u8 hwTypeOTC();        /* Independent of speed grade */
u8 hwTypeATCA();       /*      "      "    "     "   */
u8 hwTypeSRTM();       /*      "      "    "     "   */
u8 hwTypeSRTMTester(); /*      "      "    "     "   */

u32 getHWEFuse();
u8 getHWType();
u8 getHWRev();
u8 getHWVer();
u8 getHWVerRev();
u8 getHWSerialNumber();

const u32* getFuseDNARaw();
const char* getFuseDNA();
const u32* getUserRegisters();

const char* getBoardTypeName();

/* These allow us to manually override the eFuse and DNA info for debugging */
void setHWEFuse( u32 efuseValue );
void setFuseDNARaw( u32 *dnaValue );

#endif
