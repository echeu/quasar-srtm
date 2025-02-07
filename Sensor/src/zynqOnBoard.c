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
#define INCLUDE_PL_IN_PS

static const char* fsBase = "/sys/devices/platform/axi/ffa50000.ams/iio:device0";

/* The list of sensor channel numbers to actual values can be found (as of 2022.x) at
 *
 *    https://github.com/Xilinx/linux-xlnx/blob/master/Documentation/devicetree/bindings/iio/adc/xlnx%2Czynqmp-ams.yaml
 *
 * N.B. We use only channels through 29 in this document because we have no 
 * additional analog inputs. This URL also has a link to the definitions 
 * in earlier Xilinx releases.
 *
 * Channels 0 - 6 AMS
 * Channels 7 - 19 PS Sysmon
 * Channel 20 - 50 PL Sysmon
 *
 * Here is the file name list seen by 'ls -lha $fsBase' with fsBase defined above.
 *
 *   "in_voltage0_raw"    -- VCC_PSPLL
 *   "in_voltage0_scale"
 *   "in_voltage1_raw"    -- V(battery), not on SRTM. ignore
 *   "in_voltage1_scale"
 *   "in_voltage2_raw"    -- VCCINT
 *   "in_voltage2_scale"
 *   "in_voltage3_raw"    -- VCCBRAM
 *   "in_voltage3_scale"
 *   "in_voltage4_raw"    -- VCCAUX
 *   "in_voltage4_scale"
 *   "in_voltage5_raw"    -- VCC_PSDDR_PLL
 *   "in_voltage5_scale"
 *   "in_voltage6_raw"    -- VCC_PSINTFP_DDR
 *   "in_voltage6_scale"
 *   "in_temp7_input"     -- PS LPD
 *   "in_temp7_offset"
 *   "in_temp7_raw"
 *   "in_temp7_scale"
 *   "in_temp8_input"     -- temp8 Not implemented on SRTM
 *   "in_temp8_offset"
 *   "in_temp8_raw"
 *   "in_temp8_scale"
 *   "in_voltage9_raw"    -- VCC PS LPD Supply 1
 *   "in_voltage9_scale"
 *   "in_voltage10_raw"   -- VCC PS LPD Supply 2
 *   "in_voltage10_scale"
 *   "in_voltage11_raw"   -- PS AUX ref Supply 3
 *   "in_voltage11_scale"
 *   "in_voltage12_raw"   -- VCC DDR IO
 *   "in_voltage12_scale"
 *   "in_voltage13_raw"   -- PS Bank 503
 *   "in_voltage13_scale"
 *   "in_voltage14_raw"   -- PS Bank 500
 *   "in_voltage14_scale"
 *   "in_voltage15_raw"   -- VCCO_PSIO1
 *   "in_voltage15_scale"
 *   "in_voltage16_raw"   -- VCCO_PSIO2
 *   "in_voltage16_scale"
 *   "in_voltage17_raw"   -- VCC_PS_GTR
 *   "in_voltage17_scale"
 *   "in_voltage18_raw"   -- VTT_PS_GTR
 *   "in_voltage18_scale"
 *   "in_voltage19_raw"   -- VCC_PSADC (?)
 *   "in_voltage19_scale"
 *   "in_temp20_input"    -- PL
 *   "in_temp20_offset"
 *   "in_temp20_raw"
 *   "in_temp20_scale"
 *   "in_voltage21_raw"   -- PL VCCINT
 *   "in_voltage21_scale"
 *   "in_voltage22_raw"   -- PL VCCAUX
 *   "in_voltage22_scale"
 *   "in_voltage23_raw"   -- VREF_P = ?, not connected; internal voltage reference here.
 *   "in_voltage23_scale"
 *   "in_voltage24_raw"   -- VREF_N = 0, not connected
 *   "in_voltage24_scale"
 *   "in_voltage25_raw"   -- PL VCCBRAM
 *   "in_voltage25_scale"
 *   "in_voltage26_raw"   -- VCC_PSINTLP (Supply 4)
 *   "in_voltage26_scale"
 *   "in_voltage27_raw"   -- VCC_PSINTFP (Supply 5)
 *   "in_voltage27_scale"
 *   "in_voltage28_raw"   -- PS VCCAUX
 *   "in_voltage28_scale"
 *   "in_voltage29_raw"   -- PL VCCADC
 *   "in_voltage29_scale"
*/

static struct zynqDataRecord zynqDataRecs[] = {
  /* The 3 temperatures */
  {"in_temp7","PS LPD temp",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_TEMPERATURE},
  {"in_temp20","PL temp",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_TEMPERATURE},
  //{"in_temp8","PS FPD temp",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_TEMPERATURE}, /* This not on SRTM */
  /* The 27! voltages, many of which are redundant. */
  {"in_voltage0","VCC_PSPLL0",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  //{"in_voltage1","VCC_PSBATT",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},   /* This not on SRTM */
  {"in_voltage2","VCCINT",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage3","VCCBRAM",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage4","VCCAUX",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage5","VCC_PSDDRPLL",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage6","VCC_PSINTFP_DDR",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage9","VCC_PS_LPD1",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage10","VCC PS_LPD2",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage11","PS AUX3",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage12","VCC_DDR_IO",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage13","PS_Bank_503",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage14","PS_Bank_500",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage15","VCCO_PSIO1",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage16","VCCO_PSIO1",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage17","VCC_PS_GTR",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
#ifdef INCLUDE_PL_IN_PS
  {"in_voltage18","VTT_PS_GTR",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage19","VCC_PS_ADC",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage21","VCC_PL_INT",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage22","VCC_PL_AUX",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage23","VREF_P",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage24","VREF_N",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage25","VCC_PL_BRAM",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage26","VCC_PS_INTLP4",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage27","VCC_PS_INTFP5",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage28","VCC_PS_AUX",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
  {"in_voltage29","VCC_PL_ADC",1.0,0.0,0,0.0,0,ZYNQ_INTERNAL_VOLTAGE},
#endif
  {0,0,1.0,0.0,0,0.0,-1} /* EOR */
};

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
  //printf("%s in %s, found %d records\n",__FILE__,__func__,nrecs);

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
      if( cur->easyname ) cJSON_AddItemToObject(zynq,cur->easyname,cJSON_CreateNumber(cur->value));
      else cJSON_AddItemToObject(zynq,cur->basename,cJSON_CreateNumber(cur->value));
    }
    else cJSON_AddItemToObject(zynq,cur->basename,cJSON_CreateString("ERROR"));
  }
  cJSON_AddItemToObject(parent,"zynqps",zynq);

  /* And this is a LArC-like minimal "fpga" record */
  cJSON *fmt = cJSON_CreateObject();
  cJSON_AddItemToObject(parent, "fpga", fmt);
  char temp_str[10], vccint_str[10], vccaux_str[10], vBRAM_str[10];
  sprintf(temp_str,"%0.3f",zdr->zynqData[0].value);
  sprintf(vccint_str,"%0.3f",zdr->zynqData[5].value);
  sprintf(vccaux_str,"%0.3f",zdr->zynqData[7].value);
  sprintf(vBRAM_str,"%0.3f",zdr->zynqData[6].value);
  cJSON_AddNumberToObject(fmt, "up", round(zdr->uptime));
  cJSON_AddStringToObject(fmt, "temp", temp_str);
  cJSON_AddStringToObject(fmt, "vint", vccint_str);
  cJSON_AddStringToObject(fmt, "vaux", vccaux_str);
  cJSON_AddStringToObject(fmt, "vbram", vBRAM_str);

}

