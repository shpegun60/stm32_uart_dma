################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../app_core/app_core.cpp 

C_SRCS += \
../app_core/uart_communicator.c \
../app_core/uart_workers.c 

C_DEPS += \
./app_core/uart_communicator.d \
./app_core/uart_workers.d 

OBJS += \
./app_core/app_core.o \
./app_core/uart_communicator.o \
./app_core/uart_workers.o 

CPP_DEPS += \
./app_core/app_core.d 


# Each subdirectory must supply rules for building sources it contributes
app_core/%.o app_core/%.su app_core/%.cyclo: ../app_core/%.cpp app_core/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core" -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core/chaos_sdk" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app_core/%.o app_core/%.su app_core/%.cyclo: ../app_core/%.c app_core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core/tools" -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core/chaos_sdk" -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core

clean-app_core:
	-$(RM) ./app_core/app_core.cyclo ./app_core/app_core.d ./app_core/app_core.o ./app_core/app_core.su ./app_core/uart_communicator.cyclo ./app_core/uart_communicator.d ./app_core/uart_communicator.o ./app_core/uart_communicator.su ./app_core/uart_workers.cyclo ./app_core/uart_workers.d ./app_core/uart_workers.o ./app_core/uart_workers.su

.PHONY: clean-app_core

