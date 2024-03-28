/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Evan Madurai
  Created on: 15 December 2023
  Description: header file for the Barometer module MS561101BA03
*/

#ifndef MS5611_DRIVER_H
#define MS5611_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "mcu.h"

#pragma region Macros
/** @name Commands */
#define MS5611_CMD_READ_ADC       				0x00
#define MS5611_CMD_READ_PROM(address)                      (0xA0 | ((address) << 1))
#define MS5611_CMD_RESET          				0x1E
#define MS5611_CMD_CONVERT_D1     				0x40
#define MS5611_CMD_CONVERT_D2     				0x50	

/** @name Oversampling rates */
#define MS5611_PRESSURE_OSR_256  				0x40
#define MS5611_PRESSURE_OSR_512  				0x42
#define MS5611_PRESSURE_OSR_1024 				0x44
#define MS5611_PRESSURE_OSR_2048 				0x46
#define MS5611_PRESSURE_OSR_4096 				0x48

#define MS5611_TEMP_OSR_256      				0x50
#define MS5611_TEMP_OSR_512  	 				0x52
#define MS5611_TEMP_OSR_1024 	 				0x54
#define MS5611_TEMP_OSR_2048     				0x56
#define MS5611_TEMP_OSR_4096     				0x58

#define MS5611_CONVERSION_OSR_256  				1
#define MS5611_CONVERSION_OSR_512  				2
#define MS5611_CONVERSION_OSR_1024 				3
#define MS5611_CONVERSION_OSR_2048 				5
#define MS5611_CONVERSION_OSR_4096 				10

#define MS5611_CS								CS2
#pragma endregion Macros

#pragma region Structs/Emun
/**
	@brief The oversampling rate
	@note A higher value means a longer conversion
*/
typedef enum MS5611_OSR {
	MS5611_OSR_256,
	MS5611_OSR_512,
	MS5611_OSR_1024,
	MS5611_OSR_2048,
	MS5611_OSR_4096
}MS5611_OSR;

typedef struct M5611_data
{
    int32_t temp;
    int32_t pressure;
} M5611_data;
#pragma endregion Structs/Emun

/**
  @brief Initialise the pressure sensor with default parameters
*/
uint8_t MS5611_init(SPI_TypeDef* spi);

/**
  @brief Read the PROM data from the sensor
*/
uint8_t MS5611_read_PROM();

/**
  @brief Get the temperature and pressure data from the sensor
*/
uint8_t MS5611_get_data(M5611_data* data);

#endif /* MS5611_DRIVER_H */
