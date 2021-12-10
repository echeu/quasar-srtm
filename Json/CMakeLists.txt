add_library( Json OBJECT src/cJSON.c src/cJSON_Utils.c src/json_provider.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSRTM -static")