################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM3/port.c 

OBJS += \
./ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM3/%.o: ../ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM3/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\FreeRTOS\org\Source\include" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\SEGGER\Config" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\SEGGER\OS" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\SEGGER\SEGGER" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\FreeRTOS\org\Source\portable\GCC\ARM_CM3" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\Config" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\include" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\cmsis" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


