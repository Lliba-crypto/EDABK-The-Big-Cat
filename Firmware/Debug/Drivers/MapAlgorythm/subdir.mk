################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/MapAlgorythm/map_track_alg.c 

OBJS += \
./Drivers/MapAlgorythm/map_track_alg.o 

C_DEPS += \
./Drivers/MapAlgorythm/map_track_alg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/MapAlgorythm/%.o Drivers/MapAlgorythm/%.su Drivers/MapAlgorythm/%.cyclo: ../Drivers/MapAlgorythm/%.c Drivers/MapAlgorythm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test/Drivers/SSD1306" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test/Drivers/MapAlgorythm" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test/Drivers/DRV8833" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test/Drivers/ENCODER" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test/Drivers/VL53L0X" -I"C:/Users/Acer/OneDrive/Desktop/my_workspace_stm32cubeide/my_pcb_test/Drivers/MPU6050" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-MapAlgorythm

clean-Drivers-2f-MapAlgorythm:
	-$(RM) ./Drivers/MapAlgorythm/map_track_alg.cyclo ./Drivers/MapAlgorythm/map_track_alg.d ./Drivers/MapAlgorythm/map_track_alg.o ./Drivers/MapAlgorythm/map_track_alg.su

.PHONY: clean-Drivers-2f-MapAlgorythm

