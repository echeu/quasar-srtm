################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../LogIt/src/BoostRotatingFileLog.cpp \
../LogIt/src/ComponentAttributes.cpp \
../LogIt/src/LogIt.cpp \
../LogIt/src/LogItInstance.cpp \
../LogIt/src/LogLevels.cpp \
../LogIt/src/LogRecord.cpp \
../LogIt/src/LogSinks.cpp \
../LogIt/src/StdOutLog.cpp \
../LogIt/src/UaTraceSink.cpp \
../LogIt/src/WindowsDebuggerSink.cpp 

OBJS += \
./LogIt/src/BoostRotatingFileLog.o \
./LogIt/src/ComponentAttributes.o \
./LogIt/src/LogIt.o \
./LogIt/src/LogItInstance.o \
./LogIt/src/LogLevels.o \
./LogIt/src/LogRecord.o \
./LogIt/src/LogSinks.o \
./LogIt/src/StdOutLog.o \
./LogIt/src/UaTraceSink.o \
./LogIt/src/WindowsDebuggerSink.o 

CPP_DEPS += \
./LogIt/src/BoostRotatingFileLog.d \
./LogIt/src/ComponentAttributes.d \
./LogIt/src/LogIt.d \
./LogIt/src/LogItInstance.d \
./LogIt/src/LogLevels.d \
./LogIt/src/LogRecord.d \
./LogIt/src/LogSinks.d \
./LogIt/src/StdOutLog.d \
./LogIt/src/UaTraceSink.d \
./LogIt/src/WindowsDebuggerSink.d 


# Each subdirectory must supply rules for building sources it contributes
LogIt/src/%.o: ../LogIt/src/%.cpp LogIt/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


