################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AddressSpace/src/ASNodeManager.cpp \
../AddressSpace/src/ASSourceVariableIoManager.cpp \
../AddressSpace/src/ArrayTools.cpp \
../AddressSpace/src/ChangeNotifyingVariable.cpp \
../AddressSpace/src/FreeVariablesEngine.cpp 

OBJS += \
./AddressSpace/src/ASNodeManager.o \
./AddressSpace/src/ASSourceVariableIoManager.o \
./AddressSpace/src/ArrayTools.o \
./AddressSpace/src/ChangeNotifyingVariable.o \
./AddressSpace/src/FreeVariablesEngine.o 

CPP_DEPS += \
./AddressSpace/src/ASNodeManager.d \
./AddressSpace/src/ASSourceVariableIoManager.d \
./AddressSpace/src/ArrayTools.d \
./AddressSpace/src/ChangeNotifyingVariable.d \
./AddressSpace/src/FreeVariablesEngine.d 


# Each subdirectory must supply rules for building sources it contributes
AddressSpace/src/%.o: ../AddressSpace/src/%.cpp AddressSpace/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


