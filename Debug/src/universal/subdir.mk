################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/universal/universal.c 

OBJS += \
./src/universal/universal.o 

C_DEPS += \
./src/universal/universal.d 


# Each subdirectory must supply rules for building sources it contributes
src/universal/%.o: ../src/universal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -DPI_COMPILE -DPI_RUNNING -I"/home/chaunm/Working/ZigbeeHost/DemoActor/src/universal" -I"/home/chaunm/Working/ZigbeeHost/DemoActor/src/lib/jansson" -I"/home/chaunm/Working/ZigbeeHost/DemoActor/src/lib/uuid/include" -include"/home/chaunm/Working/ZigbeeHost/DemoActor/src/universal/typesdef.h" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


