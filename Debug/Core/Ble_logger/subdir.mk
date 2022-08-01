################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Ble_logger/ble_logger.c 

OBJS += \
./Core/Ble_logger/ble_logger.o 

C_DEPS += \
./Core/Ble_logger/ble_logger.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Ble_logger/%.o Core/Ble_logger/%.su: ../Core/Ble_logger/%.c Core/Ble_logger/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Ble_logger

clean-Core-2f-Ble_logger:
	-$(RM) ./Core/Ble_logger/ble_logger.d ./Core/Ble_logger/ble_logger.o ./Core/Ble_logger/ble_logger.su

.PHONY: clean-Core-2f-Ble_logger

