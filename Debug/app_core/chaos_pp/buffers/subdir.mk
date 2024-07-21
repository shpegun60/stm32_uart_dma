################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../app_core/chaos_pp/buffers/PoolContainer.cpp \
../app_core/chaos_pp/buffers/RingBase.cpp \
../app_core/chaos_pp/buffers/RingBuff.cpp 

OBJS += \
./app_core/chaos_pp/buffers/PoolContainer.o \
./app_core/chaos_pp/buffers/RingBase.o \
./app_core/chaos_pp/buffers/RingBuff.o 

CPP_DEPS += \
./app_core/chaos_pp/buffers/PoolContainer.d \
./app_core/chaos_pp/buffers/RingBase.d \
./app_core/chaos_pp/buffers/RingBuff.d 


# Each subdirectory must supply rules for building sources it contributes
app_core/chaos_pp/buffers/%.o app_core/chaos_pp/buffers/%.su app_core/chaos_pp/buffers/%.cyclo: ../app_core/chaos_pp/buffers/%.cpp app_core/chaos_pp/buffers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0plus -std=gnu++17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_pp" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_sdk" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/math" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/stm32_tools" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-chaos_pp-2f-buffers

clean-app_core-2f-chaos_pp-2f-buffers:
	-$(RM) ./app_core/chaos_pp/buffers/PoolContainer.cyclo ./app_core/chaos_pp/buffers/PoolContainer.d ./app_core/chaos_pp/buffers/PoolContainer.o ./app_core/chaos_pp/buffers/PoolContainer.su ./app_core/chaos_pp/buffers/RingBase.cyclo ./app_core/chaos_pp/buffers/RingBase.d ./app_core/chaos_pp/buffers/RingBase.o ./app_core/chaos_pp/buffers/RingBase.su ./app_core/chaos_pp/buffers/RingBuff.cyclo ./app_core/chaos_pp/buffers/RingBuff.d ./app_core/chaos_pp/buffers/RingBuff.o ./app_core/chaos_pp/buffers/RingBuff.su

.PHONY: clean-app_core-2f-chaos_pp-2f-buffers

