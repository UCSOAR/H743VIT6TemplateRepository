################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/%.o Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/%.su Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/%.cyclo: ../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/%.c Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Debug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Middlewares" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/SOARDebug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

