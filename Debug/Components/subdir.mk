################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Components/DebugTask.cpp \
../Components/RunInterface.cpp \
../Components/main_system.cpp 

OBJS += \
./Components/DebugTask.o \
./Components/RunInterface.o \
./Components/main_system.o 

CPP_DEPS += \
./Components/DebugTask.d \
./Components/RunInterface.d \
./Components/main_system.d 


# Each subdirectory must supply rules for building sources it contributes
Components/%.o Components/%.su Components/%.cyclo: ../Components/%.cpp Components/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Core" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Drivers" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Components" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/SoarOS" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Debug" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Middlewares" -I"C:/Users/Andrey/STM32CubeIDE/H743VIT6TemplateRepository/Components/SOARDebug" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components

clean-Components:
	-$(RM) ./Components/DebugTask.cyclo ./Components/DebugTask.d ./Components/DebugTask.o ./Components/DebugTask.su ./Components/RunInterface.cyclo ./Components/RunInterface.d ./Components/RunInterface.o ./Components/RunInterface.su ./Components/main_system.cyclo ./Components/main_system.d ./Components/main_system.o ./Components/main_system.su

.PHONY: clean-Components

