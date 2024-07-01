################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../app_core/tools/adc/Stm32ADCregular.cpp 

OBJS += \
./app_core/tools/adc/Stm32ADCregular.o 

CPP_DEPS += \
./app_core/tools/adc/Stm32ADCregular.d 


# Each subdirectory must supply rules for building sources it contributes
app_core/tools/adc/%.o app_core/tools/adc/%.su app_core/tools/adc/%.cyclo: ../app_core/tools/adc/%.cpp app_core/tools/adc/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core" -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core/chaos_sdk" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-tools-2f-adc

clean-app_core-2f-tools-2f-adc:
	-$(RM) ./app_core/tools/adc/Stm32ADCregular.cyclo ./app_core/tools/adc/Stm32ADCregular.d ./app_core/tools/adc/Stm32ADCregular.o ./app_core/tools/adc/Stm32ADCregular.su

.PHONY: clean-app_core-2f-tools-2f-adc

