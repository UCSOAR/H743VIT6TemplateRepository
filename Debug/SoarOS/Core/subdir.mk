################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SoarOS/Core/Command.cpp \
../SoarOS/Core/CubeUtils.cpp \
../SoarOS/Core/Mutex.cpp \
../SoarOS/Core/Queue.cpp \
../SoarOS/Core/Task.cpp \
../SoarOS/Core/Timer.cpp 

OBJS += \
./SoarOS/Core/Command.o \
./SoarOS/Core/CubeUtils.o \
./SoarOS/Core/Mutex.o \
./SoarOS/Core/Queue.o \
./SoarOS/Core/Task.o \
./SoarOS/Core/Timer.o 

CPP_DEPS += \
./SoarOS/Core/Command.d \
./SoarOS/Core/CubeUtils.d \
./SoarOS/Core/Mutex.d \
./SoarOS/Core/Queue.d \
./SoarOS/Core/Task.d \
./SoarOS/Core/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
SoarOS/Core/%.o SoarOS/Core/%.su SoarOS/Core/%.cyclo: ../SoarOS/Core/%.cpp SoarOS/Core/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_PWR_LDO_SUPPLY -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Debug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Middlewares" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/SOARDebug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Core/Src" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/TMUX1104 Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5611Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MX66L1G45GMI/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/LSM6DSODriver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/AltitudeTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash/Driver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/GPS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/covarianceCalc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/testSuite" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen/src" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen/src/Cholesky" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -u _printf_float -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SoarOS-2f-Core

clean-SoarOS-2f-Core:
	-$(RM) ./SoarOS/Core/Command.cyclo ./SoarOS/Core/Command.d ./SoarOS/Core/Command.o ./SoarOS/Core/Command.su ./SoarOS/Core/CubeUtils.cyclo ./SoarOS/Core/CubeUtils.d ./SoarOS/Core/CubeUtils.o ./SoarOS/Core/CubeUtils.su ./SoarOS/Core/Mutex.cyclo ./SoarOS/Core/Mutex.d ./SoarOS/Core/Mutex.o ./SoarOS/Core/Mutex.su ./SoarOS/Core/Queue.cyclo ./SoarOS/Core/Queue.d ./SoarOS/Core/Queue.o ./SoarOS/Core/Queue.su ./SoarOS/Core/Task.cyclo ./SoarOS/Core/Task.d ./SoarOS/Core/Task.o ./SoarOS/Core/Task.su ./SoarOS/Core/Timer.cyclo ./SoarOS/Core/Timer.d ./SoarOS/Core/Timer.o ./SoarOS/Core/Timer.su

.PHONY: clean-SoarOS-2f-Core

