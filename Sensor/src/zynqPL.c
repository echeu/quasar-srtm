/*
 * $Id: 
 *-
 *-   Purpose and Methods: 
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created  18-JUN-2023   John D. Hobbs
 *- $Revision: 1.3 $
 *-
*/

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "uio_helper.h"
#include "cJSON.h"

#include "zynqOnBoard.h"  /* Some constants */
#include "zynqPL.h"

/* Converstions and address offsets from pg-185 and ug-580 */
#define STDTEMP 0.007771514990234375
#define STDTOFFS -280.23087870
#define STDVOLT 0.00293

/* NB: The format of the name must be "PL name" and "name" will appear in the JSON record  */
static struct zynqPLDataRecord zynqPLDataRecs[] = {
  {0x400,"PL temp",STDTEMP,STDTOFFS,0,0,0,ZYNQ_INTERNAL_TEMPERATURE},
  {0x404,"PL vccint",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x408,"PL vccaux",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x410,"PL vccrefp",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x414,"PL vccrefn",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x418,"PL vccbram",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x600,"PL gthmgtavcc",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x604,"PL gthmgtavtt",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x608,"PL gtymgtavcc",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0x60C,"PL gtymgtavtt",STDVOLT,0.0,0,0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0,0,0,0,0,0,0,0}
};


static int initDone=0;
static int *baseaddr;

static int doInitIntoUserBuffer(void *valueBuffer) {
  if( initDone ) return 0;
  if( !valueBuffer ) return -3;
  
  /* Count the number of records */
  int nrecs = 0;
  while( zynqPLDataRecs[nrecs].name && nrecs<1000 /* Just safety */ ) nrecs++;

  /* and fill in the record */
  struct zynqPL *sbf = (struct zynqPL *)valueBuffer;
  sbf->nzpl = nrecs;
  sbf->zynqData = zynqPLDataRecs;
  printf("%s in %s, found %d records\n",__FILE__,__func__,nrecs);
  initDone=1;

  /* Open the UIO device */
  // Learn the device information (libuio calls)
  const char *udevname = "axiplsysmon";
  struct uio_info_t *dinfo = uio_find_devices_byname(udevname);
  //printf("Return struct address from dinfo looking for %s = 0x%08x\n",udevname,dinfo);
  if( !dinfo ) return 1;
  uio_get_all_info(dinfo);
  //printf("Device name: '%s', number = %d\n",dinfo->name,dinfo->uio_num);
  struct uio_map_t *map = dinfo->maps;
  off_t baseAddr = map->addr;
  size_t length = map->size;

  // Try using mmap with the /dev/mem physical memory device. (Needs root privs)
  size_t pagesize = sysconf(_SC_PAGE_SIZE);
  off_t page_base = (baseAddr / pagesize) * pagesize;
  off_t page_offset = baseAddr - page_base;
  //printf("Base address = 0x%08x\n",baseAddr);
  //printf("Page base = 0x%08x\n",page_base);
  //printf("Page offset = 0x%08x\n",page_offset);

  /* user space I/O access */
  char devname[32];
  sprintf(devname,"/dev/uio%d",dinfo->uio_num);
  int fd = open(devname, O_RDWR | O_SYNC);
  if( fd == (-1) ) {
	printf("Can't open %s\n",devname);
    return -1;
  }

  /* For userspace I/O access */
  unsigned char *mem = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (mem == MAP_FAILED) {
	perror("Can't map memory");
    return -2;
  }

  /* And now stash the address */
  int *mloc = (int *)(mem+page_offset);
  baseaddr = sbf->baseaddr = mloc;
  
  /* Don't repeat this */
  initDone = 1;
  return 0;
}

static void readZynqPLSensorValue( struct zynqPLDataRecord *zdr ) {
  if( !zdr ) return;
  int rvalue = *(baseaddr + zdr->regOffs/4 );  /* change offset from byte to 32 bit integer */
  zdr->raw = rvalue;
  if( zdr->type == ZYNQ_INTERNAL_TEMPERATURE ) { /* already coded for 16 bits */
    zdr->value = rvalue*zdr->scale + zdr->offset;
  }
  else {
    zdr->value = (rvalue>>6)*zdr->scale + zdr->offset;  /* Use 10 of 16 bits only */
  }
  zdr->err = 0;
}

static const char* pljsonname(const char *rawname) {
  return &rawname[3];
}

void zynqPLInit(struct sensorI2CAddress *sa) {
  /* Defer this until the user buffer is known? */
}

void zynqPLRead(struct sensorI2CAddress *sa, void *valueBuffer) {
  if( !initDone ) doInitIntoUserBuffer(valueBuffer);
  struct zynqPL *sbf = (struct zynqPL *)valueBuffer;
  if( !sbf ) return;

  /* These are the read temperatures and voltages */
  int i=0;
  for( i=0 ; i<sbf->nzpl ; i++ ) {
    struct zynqPLDataRecord *zd = &sbf->zynqData[i]; 
    if( zd ) {
      readZynqPLSensorValue( zd );
      // printf("%s (err = %d) = %f\n",zd->basename,zd->err,zd->value);
    }
  }
}

void zynqPLFormat(struct sensorRecord *sr, struct cJSON *parent) {
  if( !sr ) return;
  struct zynqPL *zdr = (struct zynqPL*)(sr->valueBuffer);
  if( !zdr ) return;

  /* This is the full PL Zynq record */
  cJSON *zynq = cJSON_CreateObject();
  for( int i=0 ; i<zdr->nzpl ; i++ ) {
    struct zynqPLDataRecord *cur = &(zdr->zynqData[i]);
    const char *jname = pljsonname(cur->name);
    if( !cur->err ) cJSON_AddItemToObject(zynq,jname,cJSON_CreateNumber(cur->value));
    else cJSON_AddItemToObject(zynq,jname,cJSON_CreateString("ERROR"));
  }
  cJSON_AddItemToObject(parent,"zynqpl",zynq);

}

