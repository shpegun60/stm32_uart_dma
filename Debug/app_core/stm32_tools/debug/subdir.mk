################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app_core/stm32_tools/debug/embedded_debug.c 

C_DEPS += \
./app_core/stm32_tools/debug/embedded_debug.d 

OBJS += \
./app_core/stm32_tools/debug/embedded_debug.o 


# Each subdirectory must supply rules for building sources it contributes
app_core/stm32_tools/debug/%.o app_core/stm32_tools/debug/%.su app_core/stm32_tools/debug/%.cyclo: ../app_core/stm32_tools/debug/%.c app_core/stm32_tools/debug/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_pp" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_sdk" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/math" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/stm32_tools" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-stm32_tools-2f-debug

clean-app_core-2f-stm32_tools-2f-debug:
	-$(RM) ./app_core/stm32_tools/debug/embedded_debug.cyclo ./app_core/stm32_tools/debug/embedded_debug.d ./app_core/stm32_tools/debug/embedded_debug.o ./app_core/stm32_tools/debug/embedded_debug.su

.PHONY: clean-app_core-2f-stm32_tools-2f-debug

