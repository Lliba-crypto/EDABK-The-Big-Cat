################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DRV8833/drv8833.c 

OBJS += \
./Drivers/DRV8833/drv8833.o 

C_DEPS += \
./Drivers/DRV8833/drv8833.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DRV8833/%.o Drivers/DRV8833/%.su Drivers/DRV8833/%.cyclo: ../Drivers/DRV8833/%.c Drivers/DRV8833/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test_v2/Drivers/SSD1306" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test_v2/Drivers/MapAlgorythm" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test_v2/Drivers/DRV8833" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test_v2/Drivers/ENCODER" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test_v2/Drivers/VL53L0X" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test_v2/Drivers/MPU6050" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-DRV8833

clean-Drivers-2f-DRV8833:
	-$(RM) ./Drivers/DRV8833/drv8833.cyclo ./Drivers/DRV8833/drv8833.d ./Drivers/DRV8833/drv8833.o ./Drivers/DRV8833/drv8833.su

.PHONY: clean-Drivers-2f-DRV8833

