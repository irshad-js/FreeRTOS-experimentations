################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/subdir.mk
-include ThirdParty/SEGGER/SEGGER/subdir.mk
-include ThirdParty/SEGGER/OS/subdir.mk
-include ThirdParty/SEGGER/Config/subdir.mk
-include ThirdParty/FreeRTOS/org/Source/portable/MemMang/subdir.mk
-include ThirdParty/FreeRTOS/org/Source/portable/GCC/ARM_CM3/subdir.mk
-include ThirdParty/FreeRTOS/org/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
FreeRTOS_SEGGER_Template.hex \

SECONDARY_SIZE += \
FreeRTOS_SEGGER_Template.siz \


# All Target
all: FreeRTOS_SEGGER_Template.elf secondary-outputs

# Tool invocations
FreeRTOS_SEGGER_Template.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g -T "C:\Projects\Eclipse_projects\FreeRTOS_SEGGER_Template\cmsis\gcc_arm.ld" -Xlinker --gc-sections -Wl,-Map,"FreeRTOS_SEGGER_Template.map" -o "FreeRTOS_SEGGER_Template.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

FreeRTOS_SEGGER_Template.hex: FreeRTOS_SEGGER_Template.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O ihex "FreeRTOS_SEGGER_Template.elf"  "FreeRTOS_SEGGER_Template.hex"
	@echo 'Finished building: $@'
	@echo ' '

FreeRTOS_SEGGER_Template.siz: FreeRTOS_SEGGER_Template.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "FreeRTOS_SEGGER_Template.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) FreeRTOS_SEGGER_Template.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
