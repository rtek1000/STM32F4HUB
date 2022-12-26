################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Host_Library/Class/HUB/usbh_hub.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/HUB/usbh_hub.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Class/HUB/usbh_hub.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Class/HUB/%.o Middlewares/ST/STM32_USB_Host_Library/Class/HUB/%.su: ../Middlewares/ST/STM32_USB_Host_Library/Class/HUB/%.c Middlewares/ST/STM32_USB_Host_Library/Class/HUB/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/CMSIS/core" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/CMSIS/device" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/HAL_Driver/Inc/Legacy" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/HAL_Driver/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Middlewares/ST/STM32_USB_Host_Library/Class/Template/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/ampire480272" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/ampire640480" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/Common" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/cs43l22" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/exc7200" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/ft6x06" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/ili9325" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/ili9341" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/l3gd20" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/lis302dl" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/lis3dsh" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/lsm303dlhc" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/mfxstm32l152" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/n25q128a" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/n25q256a" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/n25q512a" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/otm8009a" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/ov2640" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/s25fl512s" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/s5k5cag" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/st7735" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/stmpe1600" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/stmpe811" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/ts3510" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/Components/wm8994" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities" -I"/mnt/6AFEAF4BFEAF0E7F/Dados/STM32CubeIDE/workspace_1.11.0/STM32F4HUB/Utilities/STM32F4-Discovery" -O3 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-HUB

clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-HUB:
	-$(RM) ./Middlewares/ST/STM32_USB_Host_Library/Class/HUB/usbh_hub.d ./Middlewares/ST/STM32_USB_Host_Library/Class/HUB/usbh_hub.o ./Middlewares/ST/STM32_USB_Host_Library/Class/HUB/usbh_hub.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Host_Library-2f-Class-2f-HUB

