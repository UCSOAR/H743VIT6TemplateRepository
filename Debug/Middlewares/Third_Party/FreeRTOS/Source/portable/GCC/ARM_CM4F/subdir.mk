################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_PWR_LDO_SUPPLY -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Core" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Drivers" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Libraries" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Debug" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Middlewares" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/SOARDebug" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingModule" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingTask" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Core" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Core/Src" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/TMUX1104 Driver/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5611Driver/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MX66L1G45GMI/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/Components" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/LSM6DSODriver" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/AltitudePredictionFilter" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/AltitudeTask" -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F:
	-$(RM) ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.cyclo ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.d ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.o ./Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-FreeRTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM4F

