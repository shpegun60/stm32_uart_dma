################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app_core/tools/proto/stm32_uart.c 

C_DEPS += \
./app_core/tools/proto/stm32_uart.d 

OBJS += \
./app_core/tools/proto/stm32_uart.o 


# Each subdirectory must supply rules for building sources it contributes
app_core/tools/proto/%.o app_core/tools/proto/%.su app_core/tools/proto/%.cyclo: ../app_core/tools/proto/%.c app_core/tools/proto/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-tools-2f-proto

clean-app_core-2f-tools-2f-proto:
	-$(RM) ./app_core/tools/proto/stm32_uart.cyclo ./app_core/tools/proto/stm32_uart.d ./app_core/tools/proto/stm32_uart.o ./app_core/tools/proto/stm32_uart.su

.PHONY: clean-app_core-2f-tools-2f-proto

