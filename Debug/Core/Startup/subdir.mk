################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
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
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Core" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Drivers" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Components" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Debug" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Middlewares" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Components/SOARDebug" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32h743vitx.d ./Core/Startup/startup_stm32h743vitx.o

.PHONY: clean-Core-2f-Startup

