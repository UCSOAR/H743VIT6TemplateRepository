################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.c \
../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.c \
../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.c \
../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.c 

C_DEPS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.d \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.d \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.d \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.d 

OBJS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.o \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.o \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.o \
./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/%.o Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/%.su Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/%.cyclo: ../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/%.c Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -DUSE_PWR_LDO_SUPPLY -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Core" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Drivers" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Core/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Libraries" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Debug" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Middlewares" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/SOARDebug" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/LoggingModule" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/LoggingModule/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/LoggingTask" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/LoggingTask/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Components" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Components/DataBroker" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Components/SystemTypes" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/SoarOS/Components/DataBroker/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MMC5983MA_SPI" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5607Driver" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Core" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Core/Src" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/TMUX1104 Driver/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MS5611Driver/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/MX66L1G45GMI/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/Components" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/LSM6DSODriver" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/PeripheralDriversSubmodule/GPS" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/Flash/Inc" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/Flash/Driver" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Components/Flash" -I../FATFS/Target -I../FATFS/App -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/FATFS" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/USB_DEVICE" -I"C:/Users/Owner/Desktop/SOAR/H743VIT6TemplateRepository/Middlewares/Third_Party" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-MSC-2f-Src

clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-MSC-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.d ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.o ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.su ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.d ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.o ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.su ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.d ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.o ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.su ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.cyclo ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.d ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.o ./Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-MSC-2f-Src

