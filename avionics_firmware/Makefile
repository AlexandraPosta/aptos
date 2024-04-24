CFLAGS  ?=  -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion -Wno-unknown-pragmas \
            -g3 -Os -ffunction-sections -fdata-sections -I. -Iinclude \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS) \
			-lm
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c startup.c syscalls.c STM32_init.c drivers/MS5611_driver.c drivers/BME280_driver.c  \
		  drivers/ADXL375_driver.c drivers/LSM6DS3_driver.c test_routines.c data_buffer.c filters.c \
		  orientation_utils.c lqr_controller.c drivers/SERVO_driver.c kalman_filter.c

build: firmware.bin

firmware.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

dfu: firmware.bin
	STM32_Programmer_CLI -c port=usb1 --download firmware.bin 0x8000000

clean:
	del -rf firmware.*

debug:
	openocd -f "C:/Program Files/OpenOCD/openocd/scripts/board/st_nucleo_l4.cfg
