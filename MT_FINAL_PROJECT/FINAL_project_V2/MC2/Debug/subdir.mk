################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../Control_ECU.c \
../I2C_mine.c \
../Timer.c \
../external_eeprom.c \
../gpio.c \
../motor_mine.c \
../uart_mine.c 

OBJS += \
./Buzzer.o \
./Control_ECU.o \
./I2C_mine.o \
./Timer.o \
./external_eeprom.o \
./gpio.o \
./motor_mine.o \
./uart_mine.o 

C_DEPS += \
./Buzzer.d \
./Control_ECU.d \
./I2C_mine.d \
./Timer.d \
./external_eeprom.d \
./gpio.d \
./motor_mine.d \
./uart_mine.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


