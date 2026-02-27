################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
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
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Debug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Middlewares" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/SOARDebug" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Core" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Core/Src" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/TMUX1104 Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5611Driver/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MX66L1G45GMI/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/Components" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/LSM6DSODriver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/AltitudeTask" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash/Driver" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/Components/Flash/Inc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/PeripheralDriversSubmodule/GPS" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/covarianceCalc" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/testSuite" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen/src" -I"/Users/jaddina/Documents/SOAR stuff/H743VIT6TemplateRepository/AltitudePredictionFilter/Eigen/src/Cholesky" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h743vitx.d ./Core/Startup/startup_stm32h743vitx.o

.PHONY: clean-Core-2f-Startup

