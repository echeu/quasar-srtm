/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-
*/
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

#include "xtypes.h"
#include "uio_helper.h"
#include "uio_setup.h"

void* uio_setup(const char *udevname) {
  // Learn the device information (libuio calls)
  struct uio_info_t *dinfo = uio_find_devices_byname(udevname);
  /*  printf("Return struct address from dinfo looking for %s = 0x%p\n",udevname,dinfo);*/
  if( !dinfo ) return 0;
  uio_get_all_info(dinfo);
  printf("Device name: '%s', uio number = %d\n",dinfo->name,dinfo->uio_num);
  struct uio_map_t *map = dinfo->maps;
  if( !map ) {
	  printf("Error getting lvds memory map\n");
	  return 0;
  }
  off_t baseAddr = map->addr;
  size_t length = map->size;
  
  // Try using mmap with the /dev/mem physical memory device. (Needs root privs)
  size_t pagesize = sysconf(_SC_PAGE_SIZE);
  off_t page_base = (baseAddr / pagesize) * pagesize;
  off_t page_offset = baseAddr - page_base;
  printf("%s, Base address = 0x%08lx\n",udevname,baseAddr);
  printf("%s, Page base = 0x%08lx\n",udevname,page_base);
  printf("%s, Page offset = 0x%08lx\n",udevname,page_offset); fflush(stdout);
  
  /* Setup the user space I/O access */
  char devname[32];
  sprintf(devname,"/dev/uio%d",dinfo->uio_num);
  int fd = open(devname, O_RDWR | O_SYNC);
  if( fd == (-1) ) {
    printf("%s: can't open %s\n",udevname,devname);
    return 0 ;
  }
  unsigned char *mem = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (mem == MAP_FAILED) {
    perror("Can't map userspace I/O memory\n");
    return 0;
  }
  void *mloc = (void*)(mem+page_offset);
  printf("%s Memory location: 0x%p\n",udevname,mloc);
  return mloc;
}

#else

void* uio_setup(const char *udevname) {
  return 0;
}

#endif
