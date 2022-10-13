add_library( Sensor OBJECT src/adn1066.c  src/ddr4.c src/ltc2945.c src/sensorData.c  src/sfp.c src/zynqOnBoard.c src/atca48V.c  src/firefly.c  src/pca8575.c  src/sensorI2C.c src/tmp100.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSRTM -static")
