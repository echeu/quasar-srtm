################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CalculatedVariables/src/CalculatedVariable.cpp \
../CalculatedVariables/src/CalculatedVariablesChangeListener.cpp \
../CalculatedVariables/src/CalculatedVariablesEngine.cpp \
../CalculatedVariables/src/ParserVariable.cpp 

OBJS += \
./CalculatedVariables/src/CalculatedVariable.o \
./CalculatedVariables/src/CalculatedVariablesChangeListener.o \
./CalculatedVariables/src/CalculatedVariablesEngine.o \
./CalculatedVariables/src/ParserVariable.o 

CPP_DEPS += \
./CalculatedVariables/src/CalculatedVariable.d \
./CalculatedVariables/src/CalculatedVariablesChangeListener.d \
./CalculatedVariables/src/CalculatedVariablesEngine.d \
./CalculatedVariables/src/ParserVariable.d 


# Each subdirectory must supply rules for building sources it contributes
CalculatedVariables/src/%.o: ../CalculatedVariables/src/%.cpp CalculatedVariables/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


