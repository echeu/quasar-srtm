# quasar-srtm

opcUA server for the sRTM project
See the following documentation for more information:
    https://github.com/quasar-team/quasar/blob/master/Documentation/quasar.html

## Build environment

See README-docker.md for more information about setting up the docker environment. This allows one to build the executable for the target operating system (i.e. aarch64)

## Code development
- Quasar tutorials can be found here: https://www.youtube.com/channel/UCQdLb4N-CEWrpYROcC-UF6w/videos
- Adding a register for readout in the OpcUa server required modifications to the following:
    - bin/config.xml
    - Device/src/DRegs.cpp
    - Device/include/Dregs.h
    - Design/Design.xml
    - build/AddressSpace/include/ASRegs.h
    - Server/src/QuasarServer.cpp

## Building
Building the source code can be done simply by doing:
  - cd quasar-srtm
  - ./quasar.py 


## Running
Copy the whole directory structure to target board
  cd build/bin
  ./OpcUaServer

  The Unified Automation UA Client (UAExpert) can be used to communicate with the server

## Organization
  The main code is kept in the Devices subdirectory. Currently there are two files: DRegs.cpp and DRegs.h. These files allow the UA server to access information from the i2c and user defined registers. The i2c code is stored in the following subdirectories: I2c, Json, Sensor, Uio and Util.
