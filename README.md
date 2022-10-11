# quasar-srtm

opcUA server for the sRTM project
See the following documentation for more information:
  - Information about quasar, a project for developing OpcUa servers
  - https://github.com/quasar-team/quasar/blob/master/Documentation/quasar.html
  - A presentation that includes a demo for building and accessing an OpcUA server via UaExpert
  - https://indico.cern.ch/event/996093/contributions/4376616/attachments/2260131/3836369/SoCs%20for%20Detector%20Controls%20and%20their%20Applications.pdf

## Build environment

See [README-docker.md](README-docker.md) for more information about setting up the docker environment. This allows one to build the executable for the target operating system (i.e. aarch64)

## Code development
- Quasar tutorials can be found here: https://www.youtube.com/channel/UCQdLb4N-CEWrpYROcC-UF6w/videos
- Adding a register for readout in the OpcUa server required modifications to the following:
    - bin/config.xml  (contains the register address)
    - Device/src/DRegs.cpp  (the main code to read/write from the registers and i2c)
    - Device/include/Dregs.h
    - Design/Design.xml  (identifies which values to present to the client)
    - Server/src/QuasarServer.cpp  (main executable loop)
- To allow firefly values to be displayed on the opcua client just modify the following files
    - Device/src/DRegs.cpp
    - Design/Design.xml

## Building
Building the source code can be done simply by doing:
  - git clone https://github.com/echeu/quasar-srtm.git
  - cd quasar-srtm
  - ./quasar.py enable_module open62541-compat v1.3.6
  - ./quasar.py set_build_config ./open62541_config.cmake
  - ./quasar.py build


## Running
Copy the contents of the whole directory structure to the target board
- Modify the config.xml file (change the register address)
    - At this moment the address field needs to be in decimal (not hex).
- ./OpcUaServer

  The Unified Automation UA Client (UAExpert) can be used to communicate with the server

## Organization
  The main code is kept in the Devices subdirectory. Currently there are two files: DRegs.cpp and DRegs.h. These files allow the UA server to access information from the i2c and user defined registers. The i2c code is stored in the following subdirectories: I2c, Json, Sensor, Uio and Util.
