################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../app_core/tools/math/FPMovingAverage.cpp \
../app_core/tools/math/FPMovingAverageArray.cpp \
../app_core/tools/math/FPMovingAverageBase.cpp \
../app_core/tools/math/InRangeDetector.cpp \
../app_core/tools/math/PID.cpp 

OBJS += \
./app_core/tools/math/FPMovingAverage.o \
./app_core/tools/math/FPMovingAverageArray.o \
./app_core/tools/math/FPMovingAverageBase.o \
./app_core/tools/math/InRangeDetector.o \
./app_core/tools/math/PID.o 

CPP_DEPS += \
./app_core/tools/math/FPMovingAverage.d \
./app_core/tools/math/FPMovingAverageArray.d \
./app_core/tools/math/FPMovingAverageBase.d \
./app_core/tools/math/InRangeDetector.d \
./app_core/tools/math/PID.d 


# Each subdirectory must supply rules for building sources it contributes
app_core/tools/math/%.o app_core/tools/math/%.su app_core/tools/math/%.cyclo: ../app_core/tools/math/%.cpp app_core/tools/math/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core" -I"C:/Users/admin/Documents/Work/STM32/TEST/BluePill_generic_test/app_core/chaos_sdk" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app_core-2f-tools-2f-math

clean-app_core-2f-tools-2f-math:
	-$(RM) ./app_core/tools/math/FPMovingAverage.cyclo ./app_core/tools/math/FPMovingAverage.d ./app_core/tools/math/FPMovingAverage.o ./app_core/tools/math/FPMovingAverage.su ./app_core/tools/math/FPMovingAverageArray.cyclo ./app_core/tools/math/FPMovingAverageArray.d ./app_core/tools/math/FPMovingAverageArray.o ./app_core/tools/math/FPMovingAverageArray.su ./app_core/tools/math/FPMovingAverageBase.cyclo ./app_core/tools/math/FPMovingAverageBase.d ./app_core/tools/math/FPMovingAverageBase.o ./app_core/tools/math/FPMovingAverageBase.su ./app_core/tools/math/InRangeDetector.cyclo ./app_core/tools/math/InRangeDetector.d ./app_core/tools/math/InRangeDetector.o ./app_core/tools/math/InRangeDetector.su ./app_core/tools/math/PID.cyclo ./app_core/tools/math/PID.d ./app_core/tools/math/PID.o ./app_core/tools/math/PID.su

.PHONY: clean-app_core-2f-tools-2f-math

