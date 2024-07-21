################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include app_core/stm32_tools/uart/subdir.mk
-include app_core/stm32_tools/timer/subdir.mk
-include app_core/stm32_tools/stm32_printf/subdir.mk
-include app_core/stm32_tools/irq/subdir.mk
-include app_core/stm32_tools/debug/subdir.mk
-include app_core/stm32_tools/adc/subdir.mk
-include app_core/math/pid/subdir.mk
-include app_core/math/moving_averrage/subdir.mk
-include app_core/chaos_sdk/raw_parser/subdir.mk
-include app_core/chaos_sdk/printf/subdir.mk
-include app_core/chaos_sdk/preprocessor/subdir.mk
-include app_core/chaos_sdk/my_ctype/runtime_cast/subdir.mk
-include app_core/chaos_sdk/my_ctype/subdir.mk
-include app_core/chaos_sdk/hash/subdir.mk
-include app_core/chaos_sdk/crc/subdir.mk
-include app_core/chaos_sdk/convert/subdir.mk
-include app_core/chaos_sdk/callback_manager/subdir.mk
-include app_core/chaos_sdk/callback_container/subdir.mk
-include app_core/chaos_sdk/byte_order/subdir.mk
-include app_core/chaos_sdk/buffers/subdir.mk
-include app_core/chaos_sdk/assertsEngine/subdir.mk
-include app_core/chaos_pp/crc/subdir.mk
-include app_core/chaos_pp/buffers/subdir.mk
-include app_core/chaos_pp/KBus/subdir.mk
-include app_core/IFX007T_key/subdir.mk
-include app_core/subdir.mk
-include Drivers/STM32G0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := BC_Power_PWM
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BC_Power_PWM.elf \

MAP_FILES += \
BC_Power_PWM.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BC_Power_PWM.list \


# All Target
all: main-build

# Main-build Target
main-build: BC_Power_PWM.elf secondary-outputs

# Tool invocations
BC_Power_PWM.elf BC_Power_PWM.map: $(OBJS) $(USER_OBJS) C:\Users\admin\Documents\Work\STM32\Target\BC_Power_PWM\STM32G070CBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "BC_Power_PWM.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\admin\Documents\Work\STM32\Target\BC_Power_PWM\STM32G070CBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="BC_Power_PWM.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

BC_Power_PWM.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BC_Power_PWM.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) BC_Power_PWM.elf BC_Power_PWM.list BC_Power_PWM.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
