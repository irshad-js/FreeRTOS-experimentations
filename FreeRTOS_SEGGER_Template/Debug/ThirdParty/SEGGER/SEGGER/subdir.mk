################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c \
../ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

OBJS += \
./ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o \
./ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

C_DEPS += \
./ThirdParty/SEGGER/SEGGER/SEGGER_RTT.d \
./ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SEGGER/SEGGER/%.o: ../ThirdParty/SEGGER/SEGGER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\FreeRTOS\org\Source\include" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\SEGGER\Config" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\SEGGER\OS" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\SEGGER\SEGGER" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\ThirdParty\FreeRTOS\org\Source\portable\GCC\ARM_CM3" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\Config" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\include" -I"C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\cmsis" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


