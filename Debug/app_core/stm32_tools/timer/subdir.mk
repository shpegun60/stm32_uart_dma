################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../app_core/stm32_tools/timer/ProgramTimer.cpp \
../app_core/stm32_tools/timer/TimingManager.cpp 

OBJS += \
./app_core/stm32_tools/timer/ProgramTimer.o \
./app_core/stm32_tools/timer/TimingManager.o 

CPP_DEPS += \
./app_core/stm32_tools/timer/ProgramTimer.d \
./app_core/stm32_tools/timer/TimingManager.d 


# Each subdirectory must supply rules for building sources it contributes
app_core/stm32_tools/timer/%.o app_core/stm32_tools/timer/%.su app_core/stm32_tools/timer/%.cyclo: ../app_core/stm32_tools/timer/%.cpp app_core/stm32_tools/timer/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m0plus -std=gnu++17 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_pp" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/chaos_sdk" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/math" -I"C:/Users/admin/Documents/Work/STM32/Target/BC_Power_PWM/app_core/stm32_tools" -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O1 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-stm32_tools-2f-timer

clean-app_core-2f-stm32_tools-2f-timer:
	-$(RM) ./app_core/stm32_tools/timer/ProgramTimer.cyclo ./app_core/stm32_tools/timer/ProgramTimer.d ./app_core/stm32_tools/timer/ProgramTimer.o ./app_core/stm32_tools/timer/ProgramTimer.su ./app_core/stm32_tools/timer/TimingManager.cyclo ./app_core/stm32_tools/timer/TimingManager.d ./app_core/stm32_tools/timer/TimingManager.o ./app_core/stm32_tools/timer/TimingManager.su

.PHONY: clean-app_core-2f-stm32_tools-2f-timer

