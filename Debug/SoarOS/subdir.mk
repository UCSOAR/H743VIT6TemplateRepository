################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SoarOS/CubeDefines.cpp \
../SoarOS/CubeTask.cpp 

OBJS += \
./SoarOS/CubeDefines.o \
./SoarOS/CubeTask.o 

CPP_DEPS += \
./SoarOS/CubeDefines.d \
./SoarOS/CubeTask.d 


# Each subdirectory must supply rules for building sources it contributes
SoarOS/%.o SoarOS/%.su SoarOS/%.cyclo: ../SoarOS/%.cpp SoarOS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Core" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Drivers" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Components" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Debug" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Middlewares" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Components/SOARDebug" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SoarOS

clean-SoarOS:
	-$(RM) ./SoarOS/CubeDefines.cyclo ./SoarOS/CubeDefines.d ./SoarOS/CubeDefines.o ./SoarOS/CubeDefines.su ./SoarOS/CubeTask.cyclo ./SoarOS/CubeTask.d ./SoarOS/CubeTask.o ./SoarOS/CubeTask.su

.PHONY: clean-SoarOS

