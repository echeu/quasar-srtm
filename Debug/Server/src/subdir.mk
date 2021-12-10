################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Server/src/BaseQuasarServer.cpp \
../Server/src/QuasarServer.cpp \
../Server/src/QuasarServerCallback.cpp \
../Server/src/main.cpp \
../Server/src/opcserver.cpp \
../Server/src/opcserver_open62541.cpp \
../Server/src/serverconfigxml.cpp \
../Server/src/shutdown.cpp 

OBJS += \
./Server/src/BaseQuasarServer.o \
./Server/src/QuasarServer.o \
./Server/src/QuasarServerCallback.o \
./Server/src/main.o \
./Server/src/opcserver.o \
./Server/src/opcserver_open62541.o \
./Server/src/serverconfigxml.o \
./Server/src/shutdown.o 

CPP_DEPS += \
./Server/src/BaseQuasarServer.d \
./Server/src/QuasarServer.d \
./Server/src/QuasarServerCallback.d \
./Server/src/main.d \
./Server/src/opcserver.d \
./Server/src/opcserver_open62541.d \
./Server/src/serverconfigxml.d \
./Server/src/shutdown.d 


# Each subdirectory must supply rules for building sources it contributes
Server/src/%.o: ../Server/src/%.cpp Server/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


