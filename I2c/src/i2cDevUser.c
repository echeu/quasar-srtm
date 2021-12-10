/* Googled petalinux i2c example */
/* From https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841974/Linux+I2C+Driver */
/***************************** Include Files *********************************/
  
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>

#include "i2cBus.h"
#include "i2cDevUser.h"  

/************************** Constant Definitions *****************************/
  
#define I2C_TARGET_FORCE 0x0706
#define I2C_SLAVE    0x0703    /* Change slave address            */
#define I2C_FUNCS    0x0705    /* Get the adapter functionality */
#define I2C_RDWR     0x0707    /* Combined R/W transfer (one stop only)*/


static int isRestartTransaction = 0;

int I2CEnableRestart(int fd) {
  int old = isRestartTransaction;
  isRestartTransaction = 0;
  return(old);
}

int I2CDisableRestart(int fd) {
  int old = isRestartTransaction;
  isRestartTransaction = 0;
  return(old);
}


/*****************************************************************************
 *
 * Read the data from an I2C device
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

int I2CRead(int fd, Xuint8 address, int nbytes, Xuint8 *dataBuffer)
{
  Xuint8 WriteBuffer [2];    /* Buffer to hold location address.*/
  Xuint8 ReadBuffer [PAGESIZE];    /* Buffer to hold data read.*/
  Xuint8 BytesWritten;
  Xuint8 BytesRead=0;            /* Number of Bytes read from the IIC device. */
  Xuint16 OffsetAddr = 0x0;    /* Address offset in EEPROM to be written. */
  Xuint8 ReadBytes;
  int Status = 0;

  /* Set the device address... */
  unsigned long opts = I2C_TARGET_FORCE;
  if( isRestartTransaction ) opts = I2C_RDWR;  /* = TARGET_FORCE | 0x1 */
  Status = ioctl(fd, opts, address);
  if(Status < 0)
    {
      printf("\n Unable to set the device address\n");
      return -1;
    }
    
  /* Read the bytes. */
  BytesRead = read(fd, dataBuffer, nbytes);
  if(BytesRead != nbytes )
    {
      printf("I2CRead, error: Return code = %d\n",BytesRead);
      if( BytesRead==255 ) return -1;
      return BytesRead;
    }

  return BytesRead;
}
  
/*****************************************************************************/
/**
 *
 * Write data to an I2C device. The input buffer must be fully formatted!
 *
 * @param    fd - file descriptor of the i2c target
 * @param    address - I2C address of the device
 * @param    nbyte - the (total) number of bytes to write
 * @param    dataBuffer - the destination for the read write
 *
 * @return   Number of bytes written or error code if <0.
 *
 * @note     None.
 *
 ******************************************************************************/
int I2CWrite(int fd, Xuint8 addr, int nbytes, unsigned char *databytes)
{
  Xuint8 WriteBuffer[PAGESIZE + sizeof(AddressType)]; /* Buffer to hold data to be written */
  Xuint8 BytesWritten;    /* Number of Bytes written to the IIC device in one I2C xfer. */
  int    TotBytesWritten=0; /* Duh */
  Xuint16 Index;                /* Loop variable. */
  Xuint16 OffsetAddr = 0x0;    /* Address offset in EEPROM to be written. */
  Xuint8 WriteBytes;
  int Status = 0;

  /* Set the I2C device address */
  unsigned long opts = I2C_TARGET_FORCE;
  if( isRestartTransaction ) opts = I2C_RDWR;  /* = TARGET_FORCE | 0x1 */
  Status = ioctl(fd, opts, addr);
  if(Status < 0)
    {
      printf("\n Unable to set the I2C device address to 0x%04x\n",addr);
      return -1;
    }
 
  /* and do the write */
  BytesWritten = write(fd, databytes, nbytes);
  if(BytesWritten != nbytes)
    {
      printf("\nI2CWrite, error: Return code = %d, (expected nbytes = %d)\n",(int)BytesWritten,nbytes);
      if( BytesWritten==255 ) return -1;
      return BytesWritten;
    }
  return BytesWritten;
}

/*****************************************************************************/
/**
 *
 * Open an I2C device and return the file descriptor
 *
 * @param    devname - File system name of the I2C device to open
 * @return   device file descriptor
 *
 * @note     None.
 *
 ******************************************************************************/
int I2COpenDevice(char *devname) {
  return open(devname, O_RDWR);
}

int I2CCloseDevice(int fd) {
  return close(fd);
}
  
/*****************************************************************************/
/**
 *
 * Enable the MUX Switch that routes to EEPROM. Mux Reg = 0x04 for Kintex-7
 *
 * @return   None.
 *
 * @note     None.
 *
 ******************************************************************************/
  
int I2CMuxEnable(int fd, Xuint8 muxaddr, Xuint8 channel)
{
  Xuint8 WriteBuffer[2];    /* Buffer to hold location address.*/
  Xuint8 ReadBuffer = 0x0;    /* Buffer to hold data read.*/
  Xuint16 BytesToRead;
  Xuint8 BytesWritten;
  Xuint8 BytesRead;            /* Number of Bytes read from the IIC device. */
  Xuint16 Index;                /* Loop variable. */
  Xuint8 ReadBytes;
  int Status;
  
  unsigned long opts = I2C_TARGET_FORCE;
  if( isRestartTransaction ) opts = I2C_RDWR;  /* = TARGET_FORCE | 0x1 */
  Status = ioctl(fd, opts, muxaddr);
  if(Status < 0)
    {
      printf("\n Unable to set the Mux address\n");
      return -1;
    }
  
  
  /*
   * Write the MUX channel enable mask
   */
  WriteBuffer[0] = I2CMUX_CHANNELMASK(channel);
  if( geti2cDebugLevel()>0 ) printf("Selected Channel Mask = 0x%02x\n", WriteBuffer[0]);
  BytesWritten = write(fd, WriteBuffer, 1);
  if(BytesWritten != 1)
    {
      printf("Write multiplexer register mask failed\n");
      return -1;
    }
  
  BytesToRead = read(fd, &ReadBuffer, 1);
  if(BytesToRead != 1)
    {
      printf("Read multiplexer register mask failed\n");
      return -1;
    }
  if( geti2cDebugLevel()>0 ) {
    printf("Mux Reg Expected[0x%02x] Actual[0x%02x]\n", channel, ReadBuffer);
    printf("--> Mux enable successful\n");
  }
  
  return 0;
}


int I2CRegEnable(int fd, Xuint8 regaddr, Xuint8 channel) {
  Xuint8 WriteBuffer[2];    /* Buffer to hold location address.*/
  Xuint8 ReadBuffer = 0x0;    /* Buffer to hold data read.*/
  Xuint16 BytesToRead;
  Xuint8 BytesWritten;
  Xuint8 BytesRead;            /* Number of Bytes read from the IIC device. */
  Xuint16 Index;                /* Loop variable. */
  Xuint8 ReadBytes;
  int Status;
  
  unsigned long opts = I2C_TARGET_FORCE;
  if( isRestartTransaction ) opts = I2C_RDWR;  /* = TARGET_FORCE | 0x1 */
  Status = ioctl(fd, opts, regaddr);
  if(Status < 0)
    {
      printf("\n Unable to set the Reg address\n");
      return -1;
    }  
  
  /*
   * Write the REG channel enable mask
   */
  u16 mask = I2CREG_CHANNELMASK(channel);
  WriteBuffer[0] = mask & 0xFF;
  WriteBuffer[1] = (mask>>8) & 0xFF;
  //printf("I2CRegEnable, enabling channel %d with mask 0x%04x\n",channel,mask);
  if( geti2cDebugLevel()>0 ) printf("Selected Channel %d, Mask = 0x%04x\n", channel, (WriteBuffer[1]<<8) | WriteBuffer[0]);
  BytesWritten = write(fd, WriteBuffer, 2);
  if(BytesWritten != 2)
    {
      printf("Write multiplexer register mask failed\n");
      return -1;
    }
  
  BytesToRead = read(fd, &ReadBuffer, 2);
  if(BytesToRead != 2)
    {
      printf("Read multiplexer register mask failed\n");
      return -1;
    }
  if( geti2cDebugLevel()>0 ) {
    printf("Reg Reg Expected[0x%02x] Actual[0x%02x]\n", channel, ReadBuffer);
    printf("--> Reg enable successful\n");
  }
  
  return 0;
}
