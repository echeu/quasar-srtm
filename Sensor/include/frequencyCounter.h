/*
 * frequencyCounter.h
 *
 *  Created on: Nov 6, 2019
 *      Author: hobbs
 */

#ifndef SRC_FREQUENCYCOUNTER_H_
#define SRC_FREQUENCYCOUNTER_H_

#define INT32 int // The registers must be accessed only at 32 entities (minimal AXI interface)

struct fcInterface_v0 {
	INT32 config;
	INT32 base;
	INT32 counts[];
};

struct fcInterface {
	INT32 config;
  	INT32 maxval;
	INT32 base;
	INT32 counts[];
};

/* Bit masks for control register bits */
#define FCDONE 0x80000000
#define FCEN   0x20000000
#define FCCLR  0x40000000
#define FCNCLK 0xFF

#define FCMAXCNT 0x0FFFFFFF

#define NCLOCKS 10
#define MAXCNT 100000000   /* 1 sec at 100 MHz */

int resetStartWait(struct fcInterface *fci, int verbose);
int doFrequencyCounterCheck(struct fcInterface*, int);

#endif /* SRC_FREQUENCYCOUNTER_H_ */
