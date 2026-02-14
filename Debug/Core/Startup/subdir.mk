################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32h743vitx.s 

S_DEPS += \
./Core/Startup/startup_stm32h743vitx.d 

OBJS += \
./Core/Startup/startup_stm32h743vitx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Core" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Drivers" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Libraries" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Debug" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Middlewares" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/SOARDebug" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingModule" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingTask" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Core" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Core/Src" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/TMUX1104 Driver/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5611Driver/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MX66L1G45GMI/Inc" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/Components" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/PeripheralDriversSubmodule/LSM6DSODriver" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/AltitudePredictionFilter" -I"C:/Users/harry/Desktop/soar2/H743VIT6TemplateRepository/Components/AltitudeTask" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h743vitx.d ./Core/Startup/startup_stm32h743vitx.o

.PHONY: clean-Core-2f-Startup

