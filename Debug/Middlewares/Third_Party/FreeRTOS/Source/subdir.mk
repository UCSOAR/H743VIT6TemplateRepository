################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
../Middlewares/Third_Party/FreeRTOS/Source/list.c \
../Middlewares/Third_Party/FreeRTOS/Source/queue.c \
../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
../Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
../Middlewares/Third_Party/FreeRTOS/Source/timers.c 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/croutine.d \
./Middlewares/Third_Party/FreeRTOS/Source/event_groups.d \
./Middlewares/Third_Party/FreeRTOS/Source/list.d \
./Middlewares/Third_Party/FreeRTOS/Source/queue.d \
./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d \
./Middlewares/Third_Party/FreeRTOS/Source/tasks.d \
./Middlewares/Third_Party/FreeRTOS/Source/timers.d 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/croutine.o \
./Middlewares/Third_Party/FreeRTOS/Source/event_groups.o \
./Middlewares/Third_Party/FreeRTOS/Source/list.o \
./Middlewares/Third_Party/FreeRTOS/Source/queue.o \
./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o \
./Middlewares/Third_Party/FreeRTOS/Source/tasks.o \
./Middlewares/Third_Party/FreeRTOS/Source/timers.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/%.o Middlewares/Third_Party/FreeRTOS/Source/%.su Middlewares/Third_Party/FreeRTOS/Source/%.cyclo: ../Middlewares/Third_Party/FreeRTOS/Source/%.c Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_PWR_LDO_SUPPLY -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Debug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Middlewares" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/SOARDebug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Core/Src" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/TMUX1104 Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5611Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MX66L1G45GMI/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/LSM6DSODriver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/AltitudeTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash/Driver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/GPS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/covarianceCalc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/testSuite" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen/src" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen/src/Cholesky" -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/Source/croutine.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/croutine.d ./Middlewares/Third_Party/FreeRTOS/Source/croutine.o ./Middlewares/Third_Party/FreeRTOS/Source/croutine.su ./Middlewares/Third_Party/FreeRTOS/Source/event_groups.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/event_groups.d ./Middlewares/Third_Party/FreeRTOS/Source/event_groups.o ./Middlewares/Third_Party/FreeRTOS/Source/event_groups.su ./Middlewares/Third_Party/FreeRTOS/Source/list.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/list.d ./Middlewares/Third_Party/FreeRTOS/Source/list.o ./Middlewares/Third_Party/FreeRTOS/Source/list.su ./Middlewares/Third_Party/FreeRTOS/Source/queue.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/queue.d ./Middlewares/Third_Party/FreeRTOS/Source/queue.o ./Middlewares/Third_Party/FreeRTOS/Source/queue.su ./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.d ./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.o ./Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.su ./Middlewares/Third_Party/FreeRTOS/Source/tasks.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/tasks.d ./Middlewares/Third_Party/FreeRTOS/Source/tasks.o ./Middlewares/Third_Party/FreeRTOS/Source/tasks.su ./Middlewares/Third_Party/FreeRTOS/Source/timers.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/timers.d ./Middlewares/Third_Party/FreeRTOS/Source/timers.o ./Middlewares/Third_Party/FreeRTOS/Source/timers.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source

