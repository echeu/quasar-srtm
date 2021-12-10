################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/src/ASUtils.cpp \
../Common/src/QuasarThreadPool.cpp 

OBJS += \
./Common/src/ASUtils.o \
./Common/src/QuasarThreadPool.o 

CPP_DEPS += \
./Common/src/ASUtils.d \
./Common/src/QuasarThreadPool.d 


# Each subdirectory must supply rules for building sources it contributes
Common/src/%.o: ../Common/src/%.cpp Common/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


