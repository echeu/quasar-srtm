/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "uio_helper.h"

#include "frequencyCounter.h"

// Safe for both the original interface and subsequent versions
int resetStartWait(struct fcInterface *fci, int verbose) {

  // Clear the enable and clear and done bits
  int initcfg = fci->config & ~(FCDONE | FCCLR | FCEN);

  // Clear and then enable counters. This is interface version independent
  if (verbose) printf("\n\nUsing the frequency counter register structure mapped on the mmap location\n");
  fci->config = FCCLR | initcfg;
  if (verbose) printf("Control register: 0x%08x\n",fci->config);
  sleep(1);
  fci->config = initcfg;
  if (verbose) printf("Control register: 0x%08x\n",fci->config);
  sleep(1);
  fci->config = FCEN | initcfg;
  if (verbose) printf("Control register: 0x%08x\n",fci->config);
  int timeout=61;
  while( (fci->config & FCDONE) == 0 && --timeout>0 ) {
    if (verbose) printf("Control register: 0x%08x\n",fci->config);
    sleep(1);
  }
  if (verbose) printf("Control register: 0x%08x\n",fci->config);
  return timeout==0;  // return of 0 means no timeout. Return of nonzero means error
}

int doFrequencyCounterCheck(struct fcInterface* results_p, int verbose) {
    if (verbose) printf("Running frequencyCounterTest\n");

    // Learn the device information (libuio calls)
    const char *udevname = "axifreqcount";
    struct uio_info_t *dinfo = uio_find_devices_byname(udevname);
    if (verbose) printf("Return struct address from dinfo looking for %s = 0x%08x\n",udevname,dinfo);
    if( !dinfo ) return 1;
    uio_get_all_info(dinfo);
    if (verbose) printf("Device name: '%s', number = %d\n",dinfo->name,dinfo->uio_num);
    struct uio_map_t *map = dinfo->maps;
    off_t baseAddr = map->addr;
    size_t length = map->size;

    // Try using mmap with the /dev/mem physical memory device. (Needs root privs)
     size_t pagesize = sysconf(_SC_PAGE_SIZE);
     off_t page_base = (baseAddr / pagesize) * pagesize;
     off_t page_offset = baseAddr - page_base;
     if (verbose) {
      printf("Base address = 0x%08x\n",baseAddr);
      printf("Page base = 0x%08x\n",page_base);
      printf("Page offset = 0x%08x\n",page_offset);
     }

     /* user space I/O access */
     char devname[32];
     sprintf(devname,"/dev/uio%d",dinfo->uio_num);
     int fd = open(devname, O_RDWR | O_SYNC);
     if( fd == (-1) ) {
    	 printf("Can't open %s\n",devname);
    	 return -1;
     }

     // was MAP_PRIVATE instead of MAP_SHARED. msync?
     /* For root access */
     //unsigned char *mem = mmap(NULL, page_offset + length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, page_base);
     /* For userspace I/O access */
     unsigned char *mem = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
     if (mem == MAP_FAILED) {
         perror("Can't map memory");
         return -1;
     }

     unsigned int *mloc = (unsigned int *)(mem+page_offset);
     if (verbose) printf("MMap Memory location: 0x%08x\n",mloc);
     struct fcInterface *fci = (struct fcInterface*)mloc;

#ifdef CHECK_DIRECT_MAP
     // First test using the straight mmap results
     // Clear and then enable counters
     if (verbose) printf("*** Using raw mmap interface directly\n");
     *mloc = FCCLR;
	 if (verbose) printf("Control register: 0x%08x\n",*mloc);
     sleep(1);
     *mloc = 0x0;
	 if (verbose) printf("Control register: 0x%08x\n",*mloc);
     sleep(1);
     *mloc = FCEN + (MAXCNT & FCMAXCNT);
	 if (verbose) printf("Control register: 0x%08x\n",*mloc);
     int timeout=61;
     while( (*mloc & FCDONE) == 0 && --timeout>0 ) {
    	 if (verbose) printf("Control register: 0x%08x\n",*mloc);
    	 sleep(1);
     }
	 if (verbose) printf("Control register: 0x%08x\n",*mloc);

     // Print the results
     if (verbose) {
      mloc += 1;
      printf("Base Counter: %d\n",*mloc);
      for( int ic=0 ; ic<4 ; ic++ ) {
        mloc += 1;
        printf("Channel %d counter: %d\n",ic+1,*mloc);
      }
     }
#endif

     // See if it's an original interface without the number of clocks defined?
     int isOriginal=0;
     INT32 regone = *mloc;
     *mloc = 0;
     *mloc = 255;
     if( *mloc == 255 ) { // Can change the value, this is an original interface
       if (verbose) printf("fc with original interface\n");
       isOriginal = 1;
       fci->config = (fci->config & ~FCMAXCNT) + MAXCNT;
     }
     else {
       if (verbose) printf("fc with updated interface\n");
       fci->maxval = MAXCNT;
     }
    
     // Do the processing
     resetStartWait(fci, verbose);
     
     // Print/store the results
     if( isOriginal ) {
       struct fcInterface_v0 *fci0 = (struct fcInterface_v0 *)fci;
       if (verbose) {
        printf("Base Counter: %d\n",fci0->base);
        for( int ic=0 ; ic<NCLOCKS ; ic++ ) {
        printf("Channel %d counter: %d\n",ic+1,fci0->counts[ic]);
        }
       }
     }
     else {
       if (verbose) printf("Base Counter: %d\n",fci->base);
       /* copy resluts to output struct */
       /* shallow copy */
       results_p->config = fci->config;
       results_p->maxval = fci->maxval;
       results_p->base = fci->base;
       /* The only deep copy is counts[] */
       int nclk = fci->config & FCNCLK;
       for( int ic=0 ; ic<nclk ; ic++ ) {
    	  if (verbose) printf("Channel %d counter: %d\n",ic+1,fci->counts[ic]);
        /* copy this clock's freq count */
        results_p->counts[ic] = fci->counts[ic];
       }
     }

    return 0;
}
