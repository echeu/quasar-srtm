add_library( Util OBJECT src/BoardInfo.c  src/SysInfo.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSRTM -static")