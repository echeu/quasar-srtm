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

#include "zynqOnBoard.h"

#define ERR_BASENAME 1
#define ERR_SCALE 2
#define ERR_OFFSET 3
#define ERR_RAW  4

/* TODO: Auto-detect this by looking at "name" file if there is ever more than one iio device */
static const char* fsBase = "/sys/bus/iio/devices/iio:device0";

/* The following is the list of compound information for the "sensors". The commented
 * out list of the individual file name is below. Maybe later change this to read 
 * file names from the fsBase directory, and parse them on the fly to create th
 * list. But for now, just hardcode information.
 *
 * These are the files actually found in the iio device list in fsBase
    "in_temp0_ps_temp_offset",
    "in_temp0_ps_temp_raw",
    "in_temp0_ps_temp_scale",
    "in_temp1_remote_temp_offset",
    "in_temp1_remote_temp_raw",
    "in_temp1_remote_temp_scale",
    "in_temp2_pl_temp_offset",
    "in_temp2_pl_temp_raw",
    "in_temp2_pl_temp_scale",
    "in_voltage0_vcc_pspll0_raw",
    "in_voltage0_vcc_pspll0_scale",
    "in_voltage10_vccpsddr_raw",
    "in_voltage10_vccpsddr_scale",
    "in_voltage11_vccpsio3_raw",
    "in_voltage11_vccpsio3_scale",
    "in_voltage12_vccpsio0_raw",
    "in_voltage12_vccpsio0_scale",
    "in_voltage13_vccpsio1_raw",
    "in_voltage13_vccpsio1_scale",
    "in_voltage14_vccpsio2_raw",
    "in_voltage14_vccpsio2_scale",
    "in_voltage15_psmgtravcc_raw",
    "in_voltage15_psmgtravcc_scale",
    "in_voltage16_psmgtravtt_raw",
    "in_voltage16_psmgtravtt_scale",
    "in_voltage17_vccams_raw",
    "in_voltage17_vccams_scale",
    "in_voltage18_vccint_raw",
    "in_voltage18_vccint_scale",
    "in_voltage19_vccaux_raw",
    "in_voltage19_vccaux_scale",
    "in_voltage1_vcc_psbatt_raw",
    "in_voltage1_vcc_psbatt_scale",
    "in_voltage20_vccvrefp_raw",
    "in_voltage20_vccvrefp_scale",
    "in_voltage21_vccvrefn_raw",
    "in_voltage21_vccvrefn_scale",
    "in_voltage22_vccbram_raw",
    "in_voltage22_vccbram_scale",
    "in_voltage23_vccplintlp_raw",
    "in_voltage23_vccplintlp_scale",
    "in_voltage24_vccplintfp_raw",
    "in_voltage24_vccplintfp_scale",
    "in_voltage25_vccplaux_raw",
    "in_voltage25_vccplaux_scale",
    "in_voltage26_vccams_raw",
    "in_voltage26_vccams_scale",
    "in_voltage2_vccint_raw",
    "in_voltage2_vccint_scale",
    "in_voltage3_vccbram_raw",
    "in_voltage3_vccbram_scale",
    "in_voltage4_vccaux_raw",
    "in_voltage4_vccaux_scale",
    "in_voltage5_vcc_psddrpll_raw",
    "in_voltage5_vcc_psddrpll_scale",
    "in_voltage6_vccpsintfpddr_raw",
    "in_voltage6_vccpsintfpddr_scale",
    "in_voltage7_vccpsintlp_raw",
    "in_voltage7_vccpsintlp_scale",
    "in_voltage8_vccpsintfp_raw",
    "in_voltage8_vccpsintfp_scale",
    "in_voltage9_vccpsaux_raw",
    "in_voltage9_vccpsaux_scale",
*/

static struct zynqDataRecord zynqDataRecs[] = {
  /* The 3 temperatures */
  {"in_temp0_ps_temp","PS temp",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_TEMPERATURE},
  {"in_temp1_remote_temp","remote temp",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_TEMPERATURE},
  {"in_temp2_pl_temp","PL temp",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_TEMPERATURE},
  /* The 27! voltages, many of which are redundant. */
  {"in_voltage0_vcc_pspll0",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage1_vcc_psbatt",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage2_vccint",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage3_vccbram",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage4_vccaux",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage5_vcc_psddrpll",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage6_vccpsintfpddr",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage7_vccpsintlp",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage8_vccpsintfp",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage9_vccpsaux",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage10_vccpsddr",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage11_vccpsio3",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage12_vccpsio0",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage13_vccpsio1",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage14_vccpsio2",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage15_psmgtravcc",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage16_psmgtravtt",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage17_vccams",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage18_vccint",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage19_vccaux",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage20_vccvrefp",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage21_vccvrefn",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage22_vccbram",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage23_vccplintlp",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage24_vccplintfp",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage25_vccplaux",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage26_vccams",0,1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {0,0,1.0,0.0,0,0.0,-1} /* EOR */
};

static double zynq_vals[100];
static char*  zynq_names[100];

static char* readZynqConstantString( const char *basename, const char *tag ) {
  static char valueString[128]; /* Excessively long for an int or float! */
  if( !basename ) return 0;
  if( strlen(fsBase) + strlen(basename) > 255 ) return 0;
  char fname[255];
  sprintf(fname,"%s/%s_%s",fsBase,basename,tag);
  FILE *fp = fopen(fname,"r");
  if( !fp ) return 0;
  size_t nread = fread(valueString,1,127,fp);
  valueString[nread] = '0';
  fclose(fp);
  return valueString;
}

static void readZynqSensorValue( struct zynqDataRecord *zdr ) {
  if( !zdr ) return;
  if( zdr->err == ERR_OFFSET || zdr->err == ERR_SCALE ) return; /* Only if scale or offset couldn't be read */
  zdr->err = 0;
  if( !zdr->basename ) { zdr->err = ERR_BASENAME; return; }

  /* Get the raw value. For now we'll always also read the scale  */
  char *sval = readZynqConstantString(zdr->basename,"raw");
  if( !sval ) { zdr->err = ERR_RAW; return; }
  zdr->raw = atol(sval);
  zdr->value = zdr->scale*(zdr->raw + zdr->offset)/1000.0; /* Initial result in mC or mA */
  zdr->err = 0;
}

static int initDone=0;

static void doInitIntoUserBuffer(void *valueBuffer) {
  if( initDone ) return;
  if( !valueBuffer ) return;
  
  /* Count the number of records */
  int nrecs = 0;
  while( zynqDataRecs[nrecs].basename && nrecs<1000 /* Just safety */ ) nrecs++;

  /* and fill in the record */
  struct zynqOnBoard *sbf = (struct zynqOnBoard *)valueBuffer;
  sbf->nzs = nrecs;
  sbf->zynqData = zynqDataRecs;
  /*printf("%s in %s, found %d records\n",__FILE__,__func__,nrecs);*/

  /* Check text, and get the scale and (for temperatures) the offset into the record. */
  for( int i=0 ; i<nrecs ; i++ ) {
    struct zynqDataRecord *zdr = &(sbf->zynqData[i]);
    /* Make sure there is an easyname entry */
    if( !zdr->easyname ) {
      char *posn = index(zdr->basename,'_');
      posn = index(posn+1,'_');
      zdr->easyname = posn+1;
    }
    /* Get constants for the conversion */
    char *sval = readZynqConstantString(zdr->basename,"scale");
    if( !sval ) { zdr->err = 2; return; }
    zdr->scale = atof(sval);
    if( zdr->type == ZYNQ_INTERNAL_TEMPERATURE ) {
      sval = readZynqConstantString(zdr->basename,"offset");
      if( !sval ) { zdr->err = 3; return; }
      zdr->offset = atof(sval);
    }
  }

  /* Don't repeat this */
  initDone = 1;
}

void zynqOnBoardInit(struct sensorI2CAddress *sa) {
  /* The initialization is deferred until we actually have a data buffer  */
}

void zynqOnBoardRead(struct sensorI2CAddress *sa, void *valueBuffer) {
  if( !initDone ) doInitIntoUserBuffer(valueBuffer);
  struct zynqOnBoard *sbf = (struct zynqOnBoard *)valueBuffer;
  if( !sbf ) return;

  /* These are the read temperatures and voltages */
  int i=0;
  for( i=0 ; i<sbf->nzs; i++ ) {
    struct zynqDataRecord *zd = &sbf->zynqData[i]; 
    if( zd ) {
      readZynqSensorValue( zd );
      // printf("%s (err = %d) = %f\n",zd->basename,zd->err,zd->value);
    }
  }

  /* and this is the uptime, to create a LArC-like JSON record */
  sbf->uptime = -1.0;
  char valueString[128];
  FILE *fp = fopen("/proc/uptime","r");
  if( !fp ) return;
  size_t nread = fread(valueString,1,127,fp);
  fclose(fp);
  if( nread>0 ) {
    char *blank = index(valueString,' '), *dummy=0;  
    valueString[blank-valueString] = '0';
    sbf->uptime = strtod(valueString,&dummy);
  }
  else sbf->uptime = -1.0;
}

void zynqOnBoardFormat(struct sensorRecord *sr, struct cJSON *parent) {
  if( !sr ) return;
  struct zynqOnBoard *zdr = (struct zynqOnBoard *)(sr->valueBuffer);
  if( !zdr ) return;

  /* This is the full Zynq record */
  cJSON *zynq = cJSON_CreateObject();
  for( int i=0 ; i<zdr->nzs ; i++ ) {
    struct zynqDataRecord *cur = &(zdr->zynqData[i]);
    if( !cur->err ) {
      if( cur->easyname ) {
        cJSON_AddItemToObject(zynq,cur->easyname,cJSON_CreateNumber(cur->value));
	zynq_vals[i+5] = cur->value;
        zynq_names[i+5] = cur->easyname;
      }
      else {
	cJSON_AddItemToObject(zynq,cur->basename,cJSON_CreateNumber(cur->value));
        zynq_vals[i+5] = cur->value;
        zynq_names[i+5] = cur->basename;
      }
    }
    else cJSON_AddItemToObject(zynq,cur->basename,cJSON_CreateString("ERROR"));
  }
  cJSON_AddItemToObject(parent,"zynq",zynq);

  /* And this is a LArC-like minimal "fpga" record */
  cJSON *fmt = cJSON_CreateObject();
  cJSON_AddItemToObject(parent, "fpga", fmt);
  char temp_str[10], vccint_str[10], vccaux_str[10], vBRAM_str[10];
  sprintf(temp_str,"%0.3f",zdr->zynqData[0].value);
  sprintf(vccint_str,"%0.3f",zdr->zynqData[5].value);
  sprintf(vccaux_str,"%0.3f",zdr->zynqData[7].value);
  sprintf(vBRAM_str,"%0.3f",zdr->zynqData[6].value);
  cJSON_AddNumberToObject(fmt, "up", zdr->uptime);
  cJSON_AddStringToObject(fmt, "temp", temp_str);
  cJSON_AddStringToObject(fmt, "vint", vccint_str);
  cJSON_AddStringToObject(fmt, "vaux", vccaux_str);
  cJSON_AddStringToObject(fmt, "vbram", vBRAM_str);

  // ECC - store these values in the first 5 array locations
  zynq_vals[0] = zdr->uptime;
  zynq_vals[1] = zdr->zynqData[0].value;
  zynq_vals[2] = zdr->zynqData[5].value;
  zynq_vals[3] = zdr->zynqData[7].value;
  zynq_vals[4] = zdr->zynqData[6].value;
  zynq_names[0] = "up";
  zynq_names[1] = "temp";
  zynq_names[2] = "vint";
  zynq_names[3] = "vaux";
  zynq_names[4] = "vbram";

}

// ECC - function to return zynq values
//     - for now hard code in 30 values and assume that the array is big enough
void getzynqvals(double vals[], char **names){
  for (int i=0; i<35; i++) {
    vals[i] = zynq_vals[i];
    names[i] = zynq_names[i];
  }
}

