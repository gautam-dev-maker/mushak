################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Vl6180x/vl6180x.c 

OBJS += \
./Core/Vl6180x/vl6180x.o 

C_DEPS += \
./Core/Vl6180x/vl6180x.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Vl6180x/%.o Core/Vl6180x/%.su: ../Core/Vl6180x/%.c Core/Vl6180x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Vl6180x

clean-Core-2f-Vl6180x:
	-$(RM) ./Core/Vl6180x/vl6180x.d ./Core/Vl6180x/vl6180x.o ./Core/Vl6180x/vl6180x.su

.PHONY: clean-Core-2f-Vl6180x

