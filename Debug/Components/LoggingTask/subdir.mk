################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Components/LoggingTask/LoggingTask.cpp 

OBJS += \
./Components/LoggingTask/LoggingTask.o 

CPP_DEPS += \
./Components/LoggingTask/LoggingTask.d 


# Each subdirectory must supply rules for building sources it contributes
Components/LoggingTask/%.o Components/LoggingTask/%.su Components/LoggingTask/%.cyclo: ../Components/LoggingTask/%.cpp Components/LoggingTask/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Debug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Middlewares" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/SOARDebug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components-2f-LoggingTask

clean-Components-2f-LoggingTask:
	-$(RM) ./Components/LoggingTask/LoggingTask.cyclo ./Components/LoggingTask/LoggingTask.d ./Components/LoggingTask/LoggingTask.o ./Components/LoggingTask/LoggingTask.su

.PHONY: clean-Components-2f-LoggingTask

