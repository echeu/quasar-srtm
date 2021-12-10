################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include build/open62541-compat/test/src/subdir.mk
-include build/open62541-compat/src/uaclient/subdir.mk
-include build/open62541-compat/src/subdir.mk
-include build/open62541-compat/extern/open62541/src/subdir.mk
-include build/open62541-compat/CMakeFiles/open62541-compat.dir/src/uaclient/subdir.mk
-include build/open62541-compat/CMakeFiles/open62541-compat.dir/src/subdir.mk
-include build/open62541-compat/CMakeFiles/open62541-compat.dir/extern/open62541/src/subdir.mk
-include build/Server/CMakeFiles/Server.dir/src/subdir.mk
-include build/Meta/CMakeFiles/Meta.dir/src/subdir.mk
-include build/Meta/CMakeFiles/Meta.dir/__/subdir.mk
-include build/LogIt/CMakeFiles/LogIt.dir/src/subdir.mk
-include build/Device/src/subdir.mk
-include build/Device/generated/subdir.mk
-include build/Device/CMakeFiles/Device.dir/src/subdir.mk
-include build/Device/CMakeFiles/Device.dir/generated/subdir.mk
-include build/Configuration/CMakeFiles/Configuration.dir/subdir.mk
-include build/Configuration/subdir.mk
-include build/Common/CMakeFiles/Common.dir/src/subdir.mk
-include build/CalculatedVariables/CMakeFiles/CalculatedVariables.dir/src/subdir.mk
-include build/CalculatedVariables/CMakeFiles/CalculatedVariables.dir/ext_components/muparser-amalgamated/src/subdir.mk
-include build/CMakeFiles/3.20.2/CompilerIdCXX/subdir.mk
-include build/CMakeFiles/3.20.2/CompilerIdC/subdir.mk
-include build/AddressSpace/src/subdir.mk
-include build/AddressSpace/CMakeFiles/AddressSpace.dir/src/subdir.mk
-include build/subdir.mk
-include Server/src/subdir.mk
-include Meta/src/subdir.mk
-include LogIt/src/subdir.mk
-include Common/src/subdir.mk
-include CalculatedVariables/src/subdir.mk
-include CalculatedVariables/ext_components/muparser-amalgamated/src/subdir.mk
-include AddressSpace/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := opcua-server
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: opcua-server

# Tool invocations
opcua-server: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "opcua-server" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) opcua-server
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
