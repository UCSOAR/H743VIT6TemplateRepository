################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SoarOS/Profiler/ProfilerTask.cpp 

OBJS += \
./SoarOS/Profiler/ProfilerTask.o 

CPP_DEPS += \
./SoarOS/Profiler/ProfilerTask.d 


# Each subdirectory must supply rules for building sources it contributes
SoarOS/Profiler/%.o SoarOS/Profiler/%.su SoarOS/Profiler/%.cyclo: ../SoarOS/Profiler/%.cpp SoarOS/Profiler/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS/Core" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS/Drivers" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS/Libraries" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/Components" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/SoarOS" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/Debug" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/Middlewares" -I"C:/Users/Local user/Desktop/soar stuff/pee/H743VIT6TemplateRepository/Components/SOARDebug" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SoarOS-2f-Profiler

clean-SoarOS-2f-Profiler:
	-$(RM) ./SoarOS/Profiler/ProfilerTask.cyclo ./SoarOS/Profiler/ProfilerTask.d ./SoarOS/Profiler/ProfilerTask.o ./SoarOS/Profiler/ProfilerTask.su

.PHONY: clean-SoarOS-2f-Profiler

