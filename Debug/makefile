################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/universal/subdir.mk
-include src/common/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: DemoActor

# Tool invocations
DemoActor: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-linux-gnueabihf-gcc -L/home/chaunm/Working/ZigbeeHost/RpiProj/src/lib/jansson -L/home/chaunm/Working/ZigbeeHost/RpiProj/src/lib/lib-mosquitto -L/home/chaunm/Working/ZigbeeHost/RpiProj/src/lib/uuid/lib -Wl,-rpath=/home/chaunm/Working/ZigbeeHost/RpiProj/src/lib/lib-ssl -o "DemoActor" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) DemoActor
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
