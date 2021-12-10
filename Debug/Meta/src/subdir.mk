################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Meta/src/ASBuildInformation.cpp \
../Meta/src/ASComponentLogLevel.cpp \
../Meta/src/ASComponentLogLevels.cpp \
../Meta/src/ASGeneralLogLevel.cpp \
../Meta/src/ASLog.cpp \
../Meta/src/ASQuasar.cpp \
../Meta/src/ASServer.cpp \
../Meta/src/ASSourceVariableThreadPool.cpp \
../Meta/src/ASStandardMetaData.cpp \
../Meta/src/Base_DBuildInformation.cpp \
../Meta/src/Base_DComponentLogLevel.cpp \
../Meta/src/Base_DGeneralLogLevel.cpp \
../Meta/src/Base_DQuasar.cpp \
../Meta/src/Base_DServer.cpp \
../Meta/src/Base_DSourceVariableThreadPool.cpp \
../Meta/src/Base_DStandardMetaData.cpp \
../Meta/src/Certificate.cpp \
../Meta/src/DBuildInformation.cpp \
../Meta/src/DComponentLogLevel.cpp \
../Meta/src/DGeneralLogLevel.cpp \
../Meta/src/DQuasar.cpp \
../Meta/src/DServer.cpp \
../Meta/src/DSourceVariableThreadPool.cpp \
../Meta/src/DStandardMetaData.cpp \
../Meta/src/MetaBuildInfo.cpp \
../Meta/src/MetaUtils.cpp \
../Meta/src/meta.cpp 

OBJS += \
./Meta/src/ASBuildInformation.o \
./Meta/src/ASComponentLogLevel.o \
./Meta/src/ASComponentLogLevels.o \
./Meta/src/ASGeneralLogLevel.o \
./Meta/src/ASLog.o \
./Meta/src/ASQuasar.o \
./Meta/src/ASServer.o \
./Meta/src/ASSourceVariableThreadPool.o \
./Meta/src/ASStandardMetaData.o \
./Meta/src/Base_DBuildInformation.o \
./Meta/src/Base_DComponentLogLevel.o \
./Meta/src/Base_DGeneralLogLevel.o \
./Meta/src/Base_DQuasar.o \
./Meta/src/Base_DServer.o \
./Meta/src/Base_DSourceVariableThreadPool.o \
./Meta/src/Base_DStandardMetaData.o \
./Meta/src/Certificate.o \
./Meta/src/DBuildInformation.o \
./Meta/src/DComponentLogLevel.o \
./Meta/src/DGeneralLogLevel.o \
./Meta/src/DQuasar.o \
./Meta/src/DServer.o \
./Meta/src/DSourceVariableThreadPool.o \
./Meta/src/DStandardMetaData.o \
./Meta/src/MetaBuildInfo.o \
./Meta/src/MetaUtils.o \
./Meta/src/meta.o 

CPP_DEPS += \
./Meta/src/ASBuildInformation.d \
./Meta/src/ASComponentLogLevel.d \
./Meta/src/ASComponentLogLevels.d \
./Meta/src/ASGeneralLogLevel.d \
./Meta/src/ASLog.d \
./Meta/src/ASQuasar.d \
./Meta/src/ASServer.d \
./Meta/src/ASSourceVariableThreadPool.d \
./Meta/src/ASStandardMetaData.d \
./Meta/src/Base_DBuildInformation.d \
./Meta/src/Base_DComponentLogLevel.d \
./Meta/src/Base_DGeneralLogLevel.d \
./Meta/src/Base_DQuasar.d \
./Meta/src/Base_DServer.d \
./Meta/src/Base_DSourceVariableThreadPool.d \
./Meta/src/Base_DStandardMetaData.d \
./Meta/src/Certificate.d \
./Meta/src/DBuildInformation.d \
./Meta/src/DComponentLogLevel.d \
./Meta/src/DGeneralLogLevel.d \
./Meta/src/DQuasar.d \
./Meta/src/DServer.d \
./Meta/src/DSourceVariableThreadPool.d \
./Meta/src/DStandardMetaData.d \
./Meta/src/MetaBuildInfo.d \
./Meta/src/MetaUtils.d \
./Meta/src/meta.d 


# Each subdirectory must supply rules for building sources it contributes
Meta/src/%.o: ../Meta/src/%.cpp Meta/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


