################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SX1262/Src/SX1262.c 

OBJS += \
./Drivers/SX1262/Src/SX1262.o 

C_DEPS += \
./Drivers/SX1262/Src/SX1262.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SX1262/Src/%.o Drivers/SX1262/Src/%.su Drivers/SX1262/Src/%.cyclo: ../Drivers/SX1262/Src/%.c Drivers/SX1262/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I"D:/My Home Directory/Documents/Projects/Wireless Sensors/WirelessSensors/Software/CubeIDE/Debugging combined board/Drivers/SX1262/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SX1262-2f-Src

clean-Drivers-2f-SX1262-2f-Src:
	-$(RM) ./Drivers/SX1262/Src/SX1262.cyclo ./Drivers/SX1262/Src/SX1262.d ./Drivers/SX1262/Src/SX1262.o ./Drivers/SX1262/Src/SX1262.su

.PHONY: clean-Drivers-2f-SX1262-2f-Src

