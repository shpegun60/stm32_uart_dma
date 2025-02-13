################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app_core/tools/uart/stm32_uart.c \
../app_core/tools/uart/stm32_uart_container.c \
../app_core/tools/uart/uart_callbacks.c 

C_DEPS += \
./app_core/tools/uart/stm32_uart.d \
./app_core/tools/uart/stm32_uart_container.d \
./app_core/tools/uart/uart_callbacks.d 

OBJS += \
./app_core/tools/uart/stm32_uart.o \
./app_core/tools/uart/stm32_uart_container.o \
./app_core/tools/uart/uart_callbacks.o 


# Each subdirectory must supply rules for building sources it contributes
app_core/tools/uart/%.o app_core/tools/uart/%.su app_core/tools/uart/%.cyclo: ../app_core/tools/uart/%.c app_core/tools/uart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core/tools" -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core/chaos_sdk" -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-tools-2f-uart

clean-app_core-2f-tools-2f-uart:
	-$(RM) ./app_core/tools/uart/stm32_uart.cyclo ./app_core/tools/uart/stm32_uart.d ./app_core/tools/uart/stm32_uart.o ./app_core/tools/uart/stm32_uart.su ./app_core/tools/uart/stm32_uart_container.cyclo ./app_core/tools/uart/stm32_uart_container.d ./app_core/tools/uart/stm32_uart_container.o ./app_core/tools/uart/stm32_uart_container.su ./app_core/tools/uart/uart_callbacks.cyclo ./app_core/tools/uart/uart_callbacks.d ./app_core/tools/uart/uart_callbacks.o ./app_core/tools/uart/uart_callbacks.su

.PHONY: clean-app_core-2f-tools-2f-uart

