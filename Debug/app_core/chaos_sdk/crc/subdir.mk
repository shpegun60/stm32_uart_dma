################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app_core/chaos_sdk/crc/crc16.c \
../app_core/chaos_sdk/crc/crc32.c \
../app_core/chaos_sdk/crc/crc64.c \
../app_core/chaos_sdk/crc/crc8.c \
../app_core/chaos_sdk/crc/crc_base.c \
../app_core/chaos_sdk/crc/crc_factory.c \
../app_core/chaos_sdk/crc/crc_test.c 

C_DEPS += \
./app_core/chaos_sdk/crc/crc16.d \
./app_core/chaos_sdk/crc/crc32.d \
./app_core/chaos_sdk/crc/crc64.d \
./app_core/chaos_sdk/crc/crc8.d \
./app_core/chaos_sdk/crc/crc_base.d \
./app_core/chaos_sdk/crc/crc_factory.d \
./app_core/chaos_sdk/crc/crc_test.d 

OBJS += \
./app_core/chaos_sdk/crc/crc16.o \
./app_core/chaos_sdk/crc/crc32.o \
./app_core/chaos_sdk/crc/crc64.o \
./app_core/chaos_sdk/crc/crc8.o \
./app_core/chaos_sdk/crc/crc_base.o \
./app_core/chaos_sdk/crc/crc_factory.o \
./app_core/chaos_sdk/crc/crc_test.o 


# Each subdirectory must supply rules for building sources it contributes
app_core/chaos_sdk/crc/%.o app_core/chaos_sdk/crc/%.su app_core/chaos_sdk/crc/%.cyclo: ../app_core/chaos_sdk/crc/%.c app_core/chaos_sdk/crc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_pp" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_sdk" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/math" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/stm32_tools" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O1 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-chaos_sdk-2f-crc

clean-app_core-2f-chaos_sdk-2f-crc:
	-$(RM) ./app_core/chaos_sdk/crc/crc16.cyclo ./app_core/chaos_sdk/crc/crc16.d ./app_core/chaos_sdk/crc/crc16.o ./app_core/chaos_sdk/crc/crc16.su ./app_core/chaos_sdk/crc/crc32.cyclo ./app_core/chaos_sdk/crc/crc32.d ./app_core/chaos_sdk/crc/crc32.o ./app_core/chaos_sdk/crc/crc32.su ./app_core/chaos_sdk/crc/crc64.cyclo ./app_core/chaos_sdk/crc/crc64.d ./app_core/chaos_sdk/crc/crc64.o ./app_core/chaos_sdk/crc/crc64.su ./app_core/chaos_sdk/crc/crc8.cyclo ./app_core/chaos_sdk/crc/crc8.d ./app_core/chaos_sdk/crc/crc8.o ./app_core/chaos_sdk/crc/crc8.su ./app_core/chaos_sdk/crc/crc_base.cyclo ./app_core/chaos_sdk/crc/crc_base.d ./app_core/chaos_sdk/crc/crc_base.o ./app_core/chaos_sdk/crc/crc_base.su ./app_core/chaos_sdk/crc/crc_factory.cyclo ./app_core/chaos_sdk/crc/crc_factory.d ./app_core/chaos_sdk/crc/crc_factory.o ./app_core/chaos_sdk/crc/crc_factory.su ./app_core/chaos_sdk/crc/crc_test.cyclo ./app_core/chaos_sdk/crc/crc_test.d ./app_core/chaos_sdk/crc/crc_test.o ./app_core/chaos_sdk/crc/crc_test.su

.PHONY: clean-app_core-2f-chaos_sdk-2f-crc

