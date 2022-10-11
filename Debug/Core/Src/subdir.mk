################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DHT.c \
../Core/Src/ZEZ-24CXX.c \
../Core/Src/dhcp.c \
../Core/Src/dns.c \
../Core/Src/fonts.c \
../Core/Src/httpParser.c \
../Core/Src/httpUtil.c \
../Core/Src/main.c \
../Core/Src/socket.c \
../Core/Src/ssd1306.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c \
../Core/Src/w5500.c \
../Core/Src/wizchip_conf.c 

OBJS += \
./Core/Src/DHT.o \
./Core/Src/ZEZ-24CXX.o \
./Core/Src/dhcp.o \
./Core/Src/dns.o \
./Core/Src/fonts.o \
./Core/Src/httpParser.o \
./Core/Src/httpUtil.o \
./Core/Src/main.o \
./Core/Src/socket.o \
./Core/Src/ssd1306.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o \
./Core/Src/w5500.o \
./Core/Src/wizchip_conf.o 

C_DEPS += \
./Core/Src/DHT.d \
./Core/Src/ZEZ-24CXX.d \
./Core/Src/dhcp.d \
./Core/Src/dns.d \
./Core/Src/fonts.d \
./Core/Src/httpParser.d \
./Core/Src/httpUtil.d \
./Core/Src/main.d \
./Core/Src/socket.d \
./Core/Src/ssd1306.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d \
./Core/Src/w5500.d \
./Core/Src/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -u _printf_float -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DHT.d ./Core/Src/DHT.o ./Core/Src/DHT.su ./Core/Src/ZEZ-24CXX.d ./Core/Src/ZEZ-24CXX.o ./Core/Src/ZEZ-24CXX.su ./Core/Src/dhcp.d ./Core/Src/dhcp.o ./Core/Src/dhcp.su ./Core/Src/dns.d ./Core/Src/dns.o ./Core/Src/dns.su ./Core/Src/fonts.d ./Core/Src/fonts.o ./Core/Src/fonts.su ./Core/Src/httpParser.d ./Core/Src/httpParser.o ./Core/Src/httpParser.su ./Core/Src/httpUtil.d ./Core/Src/httpUtil.o ./Core/Src/httpUtil.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/socket.d ./Core/Src/socket.o ./Core/Src/socket.su ./Core/Src/ssd1306.d ./Core/Src/ssd1306.o ./Core/Src/ssd1306.su ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su ./Core/Src/w5500.d ./Core/Src/w5500.o ./Core/Src/w5500.su ./Core/Src/wizchip_conf.d ./Core/Src/wizchip_conf.o ./Core/Src/wizchip_conf.su

.PHONY: clean-Core-2f-Src

