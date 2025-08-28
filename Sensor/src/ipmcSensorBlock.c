/*
 * $Id: 
 *-
 *-   Purpose and Methods: Implement reading the Zynq internal temperature
 *-     and voltage values (from the /sys/class/iio interface) as a standard
 *-     sensor in our sensor library. This code does not use the i2c bus, but
 *-     it does get the storage address from the input address struct.
 *-
 *-   Inputs  :
 *-   Outputs :
 *-   Controls:
 *-
 *-   Created  17-MAY-2021   John D. Hobbs
 *- $$
 *-
*/

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "assert.h"
#include "time.h"

#include "ipmcSensorBlock.h"

/* These are defined in the IPMC code in the HL-IPMC repo*/
#define ID_SRTM_TESTER 2
#define ID_LASP 3


static const char* bufferFilename = "/sys/bus/i2c/devices/0-1074/zynq-buffer";
static FILE *fp = 0;
static int initDone=0;

enum ERRCODE { ERR_NONE=0, ERR_UNK=1, ERR_NYI=2};
#define ERRMSG(A) { printf("%s:%d, ERROR: %s\n",__FUNCTION__,__LINE__,(A)); } 

void SrtmTester_v1_decoder(struct ipmcDataRecord *idr, struct cJSON *ipmc);

static void addHeaderDataToJSON(struct ipmcDataHeader *header, struct cJSON *ipmc) {
  cJSON_AddItemToObject(ipmc,"id",cJSON_CreateNumber(header->boardid));
  cJSON_AddItemToObject(ipmc,"i2cVer",cJSON_CreateNumber(header->i2cBlockVer));
  cJSON_AddItemToObject(ipmc,"rev",cJSON_CreateNumber(header->ipmcRev));
  cJSON_AddItemToObject(ipmc,"ver",cJSON_CreateNumber(header->ipmcVer));
  cJSON_AddItemToObject(ipmc,"seq",cJSON_CreateNumber(header->seqid));
  cJSON_AddItemToObject(ipmc,"status",cJSON_CreateNumber(header->status));
}

static unsigned long makeTimeStamp(u8 *tsraw) {
  unsigned long ts = 0;
  for( int i=0 ; i<8 ; i++ ) ts = (ts<<8) | tsraw[7-i];  /* Watch endianness here. */
  ts = ts/1000000000; /* ns to sec */
  return ts;
}
/* This is the switch table which chooses the decoder and struct setup based on board type and ipmc record version. 
   Figure out which decoder to use. The board type and version are "permanent" in that a shutdown and power cycle 
   will be done if a board changes.
  */
int setupDecoder(struct ipmcDataRecord *sbf) {
  if( !sbf->decoder ) {
    switch ( sbf->idh.boardid ) {
      case ID_SRTM_TESTER:
	      sbf->values = 0;
	      sbf->decoder = SrtmTester_v1_decoder;
        break;
      case ID_LASP:
        /* TODO: Implement a LASP decoder. */
        return ERR_NYI;  
      default:
        return ERR_UNK;
    } 
  }
  return ERR_NONE;
}

/* A header record of all 0xFF indicates an invalid record. */
static void resetHeader(u8 *header) {
  memset(header,0xFF,sizeof(struct ipmcDataHeader)); /* Empty record. */
}

/* -----------------------------------------------------------------------------
 * The next 3 functions implement the mandatory sensor access interface. See
 * struct sensor I2CAddress (bad name) and struct sensorRecord in sensorData.h. 
 * Only the function part of the interface defined in the struct is used here. */

/* Open the memory buffer for binary reading. */
void ipmcSensorBlockInit(struct sensorI2CAddress *sa) {
  if( initDone ) return;

  //fp = fopen(bufferFilename,"rb");
  //if( !fp ) {
  //  /* TODO: Figure out how to signal an error. */
  //  return;
  //}

  initDone = 1;
}

void ipmcSensorBlockRead(struct sensorI2CAddress *sa, void *valueBuffer) {
  if( !initDone ) ipmcSensorBlockInit(sa);

  struct ipmcDataRecord *sbf = (struct ipmcDataRecord*)valueBuffer;
  struct ipmcDataHeader *header = &(sbf->idh);
  if( ! sbf ) {
    /* TODO: Figure out how to signal an error. */
    return;
  }

  /* Get the header */
  fp = fopen(bufferFilename,"rb");
  if( !fp ) {
    /* TODO: Figure out how to signal an error. */
    return;
  }

  fseek(fp,0,SEEK_SET);
  int rc = fread(header,1,sizeof(struct ipmcDataHeader),fp);
  if( rc != sizeof(struct ipmcDataHeader)) {
    /* TODO: Figure out how to signal an error. */
    resetHeader((u8*)header);
    fclose(fp);
    return;
  }

  /*  printf("Seq = 0x%02x\n",header->seqid); */

  /* The first time through, set up the values array. This needs the number of words = number of bytes. */
  if( !sbf->values ) {
    sbf->values = malloc(sizeof(double)*(header->nBytes));
    if( !sbf->values ) {
      resetHeader((u8*)header);
      fclose(fp);
      return;
    }
  }

  /* And read the raw sensor data. */
  if( !sbf->rawSensorData ) sbf->rawSensorData = malloc(header->nBytes); /* "permanently" allocated. Changing board type or id requires a power cycle anyway. */
  u8 *rawdata = sbf->rawSensorData;
  if( !rawdata ) {
    /* TODO: Figure out how to signal an error. */
    fclose(fp);
    return;
  }
  rc = fread(rawdata,1,header->nBytes,fp);
  if( rc != header->nBytes ) {
    /* TODO: Figure out how to signal an error. */
    fclose(fp);
    return;
  }

  /* and finally the sequence id and time stamp at the end. */
  u8 trailer[9];
  rc = fread(trailer,1,9,fp);
  if( rc != 9 ) {
    /* TODO: Figure out how to signal an error. */
    fclose(fp);
    return;
  }

  /* If the sequence number has changed, the record is invalid. */
  if( header->seqid != trailer[0] ) {
    resetHeader((u8*)header);
    fclose(fp);
    return;
  }

  /* And recheck the starting sequence number*/
  u8 tmphdr[sizeof(struct ipmcDataHeader)];
  fseek(fp,0,SEEK_SET);
  rc = fread(tmphdr,1,sizeof(struct ipmcDataHeader),fp);
  if( header->seqid != tmphdr[0] ) {
    resetHeader((u8*)header);
    fclose(fp);
    return;
  }

  /* Copy the timestamp from the trailer into the data record. */
  memcpy(sbf->timestamp,&trailer[1],8);
  fclose(fp);
}

void ipmcSensorBlockFormat(struct sensorRecord *sr, struct cJSON *parent) {
  if( !sr ) return;
  if( !parent ) return;

  struct ipmcDataRecord *sbf = (struct ipmcDataRecord*)(sr->valueBuffer);
  if( !sbf ) return;
  if( !sbf->rawSensorData ) return;
  if( !sbf->decoder ) {
    int rc = setupDecoder(sbf);
    if( rc ) {
      ERRMSG("no decoder\n");
      return;
    }
  }

  /* This is the full IPMC record */
  cJSON *ipmc = cJSON_CreateObject();
  if(sbf->decoder !=  SrtmTester_v1_decoder) {
    char buf[1024];
    sprintf(buf,"Function directly, %p\nDecoder pointer,   %p\n",SrtmTester_v1_decoder,sbf->decoder);
    ERRMSG(buf);
    return;
  }
  sbf->decoder(sbf,ipmc);
  cJSON_AddItemToObject(parent,"ipmc",ipmc);
}

/* ------------------------------------------------------------------------- */
/* The remaining structs and funcs are unique for a given board type and
   sensor format. The board types are defind in the IPMC code in the repo
   HL-IPMC. Each board type (and format version} have their own decoder
   ------------------------------------------------------------------------- */

 struct sensorConverter {
  const char *tag;
  const char *name;
  double scale;
  double offset;
  double data;  /* Only valid after a conversion is done. */
};
  
/* boardid = SrtmTester and i2cBlockVer=1. The order in this structure must 
 * examctly match the order in the sensor block itself. Each raw entry is one 
 * byte, and the tag value must be unique to insert it in the JSON record.  */

struct sensorConverter SrtmTester_v1_ConvertedData[] = {
  {"iQ65033qma10_I","48V current",23.97/255.},
  {"iQ65033qma10_VA","48 V voltage (A}",82.875/255.},
  {"iQ65033qma10_VB","48 V voltage (B}",82.875/255.},
  {"iQ65033qma10_T","48 V brick temperature",(450.055-(-50.))/255,-50.},
  {"PCF8575_al","SRTM status Low Byte",1},  /* These four are a kludge. Bit fields*/
  {"PCF8575_ah","SRTM status High Byte",1},
  {"PCF8575_bl_V","Low Byte",1},
  {"PCF8575_bh_I","High Byte",1},
  {"TMP100_fb","Front Bottom Temperature",127.5/255.},
  {"TMP100_bb","Back Bottom Temperature",127.5/255.},
  {"TMP100_ft","Front Top Temperature",127.5/255.},
  {"TMP100_bt","Back Top Temperature",127.5/255.},
  {"TMP100_z","SRTM Zynq Temperature",127.5/255.},
  {"LTC2945_6A_V","SRTM 12V, V",25.5/255.},
  {"LTC2945_6A_I","SRTM 12V, I",25.5/255.},
  {"LTC2945_6A_T","SRTM 12V, T",127.5/255.},
  {"LTC2945_67_V","67 ATCA 12V, V",25.5/255.},
  {"LTC2945_67_I","67 ATCA 12V, I",25.5/255.},
  {"LTC2945_6B_V","6B 12V, V",25.5/255.},
  {"LTC2945_6B_I","6B 12V, I",25.5/255.},
  {"LTC2945_6D_V1","5V, V",6.375/255.},
  {"LTC2945_6D_I1","5V, I",25.5/255.},
  {"LTC2945_69_V","3.3V, V",6.375/255.},
  {"LTC2945_69_I","3.3V, I",25.5/255.},
  {"LTC2945_6C_V","FF 3.3V, V",6.375/255.},
  {"LTC2945_6C_I","FF 3.3V, I",25.5/255.},
  {"LTC2945_6D_V2","1.2V, V",6.375/255.},
  {"LTC2945_6D_I2","1.2V, I",25.5/255.},
  {"LTC2945_6E_V","2.5V, V",6.375/255.},
  {"LTC2945_6E_I","2.5V, I",25.5/255.},
  {0,0}  /* A null tag indicates the end of the record. */
};

void SrtmTester_v1_decoder(struct ipmcDataRecord *idr, struct cJSON *ipmc) {
  
  /* First add header information. */
  struct ipmcDataHeader *header = &(idr->idh);
  addHeaderDataToJSON(header,ipmc);
  unsigned long ts = makeTimeStamp(idr->timestamp);
  cJSON_AddItemToObject(ipmc,"rawtime",cJSON_CreateNumber(ts));
  cJSON_AddItemToObject(ipmc,"time",cJSON_CreateString(ctime(&ts)));

  /* Now the sensor date. */
  struct sensorConverter *current = SrtmTester_v1_ConvertedData;
  int idxraw = 0;
  int nBytes = idr->idh.nBytes;
  u8 *dataraw = idr->rawSensorData;
#ifdef DEBUG_DUMP
  char ebuf[1024];
  sprintf(ebuf,"dataraw: %p; nbytes: %d",dataraw,nBytes);
  ERRMSG(ebuf);
#endif
  while( current->tag ) {
    /* TODO: Decide whether or not to remove the data entry from the struct. Also stored in values. */
    current->data = current->scale * dataraw[idxraw] + current->offset;
    if( idr->values ) { /* TODO: Set up converted value local storage */
      if( idxraw < nBytes ) idr->values[idxraw] = current->data; /* The "if" is just for safety. */
      else {
      	printf("LOGIC ERROR: %c:%d, too many sensors. nBytes = %d, idxraw = %d (tag = %s)\n",__FILE__,__LINE__,nBytes,idxraw,current->tag);
	      /* TODO: Figure out how to send an error. Maybe use kprintf? */
      }
    }
    cJSON_AddItemToObject(ipmc,current->tag,cJSON_CreateNumber(current->data));
    idxraw += 1;
    ++current;
  }
  if ( idxraw != idr->idh.nBytes ) {
    printf("Unexpected record count. idxraw = %d, nBytes = %d\n",idxraw,idr->idh.nBytes);
    return;
  }
  return;
}

