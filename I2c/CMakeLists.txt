add_library( I2c OBJECT src/i2cBus.c  src/i2cDevUser.c  src/i2cEEPROM.c  src/i2cFTDIUser.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSRTM -static")