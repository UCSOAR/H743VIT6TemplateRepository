################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include SoarOS/Drivers/subdir.mk
-include SoarOS/Core/subdir.mk
-include SoarOS/subdir.mk
-include PeripheralDriversSubmodule/TMUX1104\ Driver/subdir.mk
-include PeripheralDriversSubmodule/NAU7802/subdir.mk
-include PeripheralDriversSubmodule/MS5611Driver/subdir.mk
-include PeripheralDriversSubmodule/MS5607Driver/subdir.mk
-include PeripheralDriversSubmodule/MMC5983MA_SPI/subdir.mk
-include PeripheralDriversSubmodule/LSM6DSODriver/subdir.mk
-include PeripheralDriversSubmodule/GPS/subdir.mk
-include PeripheralDriversSubmodule/Components/subdir.mk
-include PeripheralDriversSubmodule/.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/subdir.mk
-include PeripheralDriversSubmodule/.metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Components/LoggingTask/subdir.mk
-include Components/LoggingModule/subdir.mk
-include Components/Flash/Driver/subdir.mk
-include Components/Flash/subdir.mk
-include Components/AltitudeTask/subdir.mk
-include Components/subdir.mk
-include AltitudePredictionFilter/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := H743VIT6TemplateRepository
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
H743VIT6TemplateRepository.elf \

MAP_FILES += \
H743VIT6TemplateRepository.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
H743VIT6TemplateRepository.list \


# All Target
all: main-build

# Main-build Target
main-build: H743VIT6TemplateRepository.elf secondary-outputs

# Tool invocations
H743VIT6TemplateRepository.elf H743VIT6TemplateRepository.map: $(OBJS) $(USER_OBJS) C:\Users\harry\Desktop\soar2\H743VIT6TemplateRepository\STM32H743VITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "H743VIT6TemplateRepository.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\harry\Desktop\soar2\H743VIT6TemplateRepository\STM32H743VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="H743VIT6TemplateRepository.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

H743VIT6TemplateRepository.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "H743VIT6TemplateRepository.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) H743VIT6TemplateRepository.elf H743VIT6TemplateRepository.list H743VIT6TemplateRepository.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
