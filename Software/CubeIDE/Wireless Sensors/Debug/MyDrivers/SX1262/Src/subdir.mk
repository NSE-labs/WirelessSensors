################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyDrivers/SX1262/Src/SX1262.c 

OBJS += \
./MyDrivers/SX1262/Src/SX1262.o 

C_DEPS += \
./MyDrivers/SX1262/Src/SX1262.d 


# Each subdirectory must supply rules for building sources it contributes
MyDrivers/SX1262/Src/%.o MyDrivers/SX1262/Src/%.su MyDrivers/SX1262/Src/%.cyclo: ../MyDrivers/SX1262/Src/%.c MyDrivers/SX1262/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L151xB -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"D:/My Home Directory/Documents/Projects/Wireless Sensors/WirelessSensors/Software/CubeIDE/Wireless Sensors/MyDrivers/SX1262/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyDrivers-2f-SX1262-2f-Src

clean-MyDrivers-2f-SX1262-2f-Src:
	-$(RM) ./MyDrivers/SX1262/Src/SX1262.cyclo ./MyDrivers/SX1262/Src/SX1262.d ./MyDrivers/SX1262/Src/SX1262.o ./MyDrivers/SX1262/Src/SX1262.su

.PHONY: clean-MyDrivers-2f-SX1262-2f-Src

