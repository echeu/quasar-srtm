

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//#include "hexdump.h"
#include "i2cBus.h"
#include "i2cEEPROM.h"

/*****************************************************************************
 *
 * Read the data from the I2C EEPROM on the ZCU111.
 *
 * @param    fd - file descriptor of the i2c target
 * @param    address - I2C address of the device
 * @param    nbyte - the (total) number of bytes to read
 * @param    dataBuffer - the destination for the read data
 *
 * @return   Status of the read from EEPROM
 *
 * @note     None.
 *
 ******************************************************************************/  

int I2CEepromRead(int fd, Xuint8 address, int nbytes, Xuint8 *dataBuffer)
{
  Xuint8 WriteBuffer [2];    /* Buffer to hold location address.*/
  Xuint8 ReadBuffer [PAGESIZE];    /* Buffer to hold data read.*/
  Xuint8 BytesWritten;
  Xuint8 BytesRead=0;            /* Number of Bytes read from the IIC device. */
  Xuint16 OffsetAddr = 0x0;    /* Address offset in EEPROM to be written. */
  Xuint8 ReadBytes;
  int Status = 0;

  OffsetAddr = 0;
  if(sizeof(AddressType) == 1)
    {
      WriteBuffer[0] = (Xuint8)(OffsetAddr);
    }
  else
    {
      WriteBuffer[0] = (Xuint8)(OffsetAddr >> 8);
      WriteBuffer[1] = (Xuint8)(OffsetAddr);
    }
  
  /*
   * Position the address pointer in EEPROM.
   */

  BytesWritten = I2CWrite(fd, address, sizeof(AddressType), WriteBuffer);
  
  /*
   * limit the bytes to be read to the Page size.
   */
  if(nbytes > PAGESIZE)
    {
      ReadBytes = PAGESIZE;
    }
  else
    {
      ReadBytes = nbytes;
    }
  
  /*
   * Read the bytes.
   */
  int totBytes=0;
  while(nbytes > 0)
    {
      printf ("Performing Read operation\n");
      BytesRead = I2CRead(fd, address, nbytes, ReadBuffer);
      if(BytesRead != ReadBytes)
        {
	  printf("\nITP_IIC_TEST1: Test Failed in read\n");
  
	  return -1;
        }

      int ib=0;
      for( ib=0 ; ib<BytesRead ; ib++ ) dataBuffer[totBytes+ib] = ReadBuffer[ib];
      /*
       * Update the read counter.
       */
      nbytes -= BytesRead;
      if(nbytes > PAGESIZE)
        {
	  ReadBytes = PAGESIZE;
        }
      else
        {
	  ReadBytes = nbytes;
        }
  
    }
  
  printf("\nRead EEPROM Succesful\n");
  
  return 0;
}
  
/*****************************************************************************/
/**
 *
 * Write data to the EEPROM on the ZCU111.
 *
 * @param    fd - file descriptor of the i2c target
 * @param    address - I2C address of the device
 * @param    nbyte - the (total) number of bytes to write
 * @param    dataBuffer - the destination for the read write
 *
 * @return   None.
 *
 * @note     None.
 *
 ******************************************************************************/
int I2CEepromWrite(int fd, Xuint8 addr, int nbytes, unsigned char *databytes)
{
  Xuint8 WriteBuffer[PAGESIZE + sizeof(AddressType)]; /* Buffer to hold data to be written */
  Xuint8 BytesWritten;    /* Number of Bytes written to the IIC device in one I2C xfer. */
  int    TotBytesWritten=0; /* Duh */
  Xuint16 Index;                /* Loop variable. */
  Xuint16 OffsetAddr = 0x0;    /* Address offset in EEPROM to be written. */
  Xuint8 WriteBytes;
  int Status = 0;

  OffsetAddr = 0;
  /* Load the offset address inside EEPROM where data need to be written. Is this correct? */
  if(sizeof(AddressType) == 1)
    {
      WriteBuffer[0] = (Xuint8)(OffsetAddr);
    }
  else
    {
      WriteBuffer[0] = (Xuint8)(OffsetAddr >> 8);
      WriteBuffer[1] = (Xuint8)(OffsetAddr);
    }
    
  /*
   * Limit the number of bytes per cycle to the page size. JDH: This can probably be improved later
   */
  if(nbytes > PAGESIZE)
    {
      WriteBytes = PAGESIZE + sizeof(AddressType);
    }
  else
    {
      WriteBytes = nbytes + sizeof(AddressType);
    }

  /* Copy the first chunk of data into the local write buffer. */
  int i=0;
  for( i=0 ; i<WriteBytes ; i++ ) WriteBuffer[i+sizeof(AddressType)] = databytes[i];
  
  while(nbytes > 0)
    {
      BytesWritten = I2CWrite(fd, addr, WriteBytes, WriteBuffer);
      sleep(1);   // Wait 'til the device internally completes the write cycle
      if(BytesWritten != WriteBytes)
        {
	  printf("\nEEPROM write failed: nbytes = %d, return code = %d\n",nbytes,(int)BytesWritten);
	  return -2;
        }
      /*
       * Update the write counter.
       */
      nbytes -= (BytesWritten - sizeof(AddressType));
      if(nbytes > PAGESIZE)
        {
	  WriteBytes = PAGESIZE + sizeof(AddressType);
        }
      else
        {
	  WriteBytes = nbytes + sizeof(AddressType);
        }
  
      /*
       * Increment the offset address.
       */
      OffsetAddr += PAGESIZE;
  
      /*
       * load the new offset address.
       */
      if(sizeof(AddressType) == 1)
        {
	  WriteBuffer[0] = (Xuint8)(OffsetAddr);
        }
      else
        {
	  WriteBuffer[0] = (Xuint8)(OffsetAddr >> 8);
	  WriteBuffer[1] = (Xuint8)(OffsetAddr);
	}
  
      /*
       * load the new data into buffer.
       */
      for(Index = 0; Index < PAGESIZE; Index++)  WriteBuffer[Index + sizeof(AddressType)] = databytes[Index+OffsetAddr];
    }
  
  printf("\nWrite EEPROM Succesful\n");
  
  return 0;
}

#ifdef STANDALONE_TEST
/**************************** Function Definitions ***************************/
  
/*****************************************************************************/
/**
 *
 * Entry point for integration tests on IIC.
 *
 * @param    .
 *
 * @return   0 if successful else -1.
 *
 * @note     None.
 *
 ******************************************************************************/
int main()
{
  doI2cEEPromTest();
}
#endif

#define ALLOW_I2C_TEST
#ifdef ALLOW_I2C_TEST
/*****************************************************************************/
/**
 * This is the linux example for i2c.
 * Note:    10-bit addressing is not supported in the current linux driver.
 * Repeated start also not supported in the current linux driver.
 *
 * @note     None.
 *
 ******************************************************************************/
int doI2cEEPromTest() {
  int Status;
  int Index;
  
  /*
   * Open the device.
   */
  int Fdiic = I2COpenDevice("/dev/i2c-1");
  if(Fdiic < 0)
    {
      printf("Cannot open the IIC device\n");
      return 1;
    }
  printf("I2C file descriptor = %d\n",Fdiic);
  
  Xuint8 mask = I2CMUX_CHANNELMASK(MUX_CHANNEL_KINTEX7);
  printf("MUX channel (%d) mask = 0x%02x\n",MUX_CHANNEL_KINTEX7,mask);
  Status = I2CMuxEnable(Fdiic,  MUX_ADDR, mask);
  if (Status)
    {
      printf("Cannot Enable Mux device\n");
      close(Fdiic);
      return 1;
    }

  /* Load the write data  */
  Xuint8 data[PAGESIZE];
  for(Index = 0; Index < PAGESIZE; Index++) data[Index] = 0;
  Status = I2CEepromWrite(Fdiic,EEPROM_ADDR,PAGESIZE,data);
  if (Status)
    {
      printf("Cannot Write into EEPROM device\n");
      close(Fdiic);
      return 1;
    }
  for(Index = 0; Index < PAGESIZE; Index++) data[Index] = 0;
  Status = I2CEepromRead(Fdiic,EEPROM_ADDR,PAGESIZE,data);
  if (Status)
    {
      printf("Cannot Read from EEPROM \n");
      close(Fdiic);
      return 1;
    }
  //hexdump(data,PAGESIZE);

  for(Index = 0; Index < PAGESIZE; Index++) data[Index] = 0x30 + Index;
  Status = I2CEepromWrite(Fdiic,EEPROM_ADDR,PAGESIZE,data);
  if (Status)
    {
      printf("Cannot Write into EEPROM device. Error code = %d\n",Status);
      close(Fdiic);
      return 1;
    }
  
  for(Index = 0; Index < PAGESIZE; Index++) data[Index] = 0;
  Status = I2CEepromRead(Fdiic,EEPROM_ADDR,PAGESIZE,data);
  if (Status)
    {
      printf("Cannot Read from EEPROM \n");
      close(Fdiic);
      return 1;
    }
  //hexdump(data,PAGESIZE);
  
  printf("eeprom test successful \n");
  I2CCloseDevice(Fdiic);
  return 0;
}
#endif
