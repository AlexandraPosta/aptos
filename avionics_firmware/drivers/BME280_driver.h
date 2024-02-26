/*
	Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 10 June 2023
  Description: header file for the Pressure/Temp/humidity module BME280 (https://www.mouser.co.uk/datasheet/2/783/bst_bme280_ds002-2238172.pdf)
*/
#ifndef BME280_DRIVER_H
#define BME280_DRIVER_H
#include "stdint.h"

#pragma region Macros
/** @name BME280 chip status */

/** @name BME280 chip identifier */
#define BME280_CHIP_ID                            0x60

/** @name Register Address */
#define BME280_REG_CHIP_ID                        0xD0
#define BME280_REG_RESET                          0xE0
#define BME280_REG_TEMP_PRESS_CALIB_DATA          0x88
#define BME280_REG_HUMIDITY_CALIB_DATA            0xE1
#define BME280_REG_CTRL_HUM                       0xF2
#define BME280_REG_STATUS                         0xF3
#define BME280_REG_PWR_CTRL                       0xF4
#define BME280_REG_CTRL_MEAS                      0xF4
#define BME280_REG_CONFIG                         0xF5
#define BME280_REG_DATA                           0xF7

/** @name Commands */
#define BME280_CMD_SOFT_RESET                     0xB6
#define BME280_CMD_STATUS_IM_UPDATE               0x01

/** @name Measurement delay calculation macros  */
#define BME280_STARTUP_DELAY                      2000

/** @name Macros related to size */
#define BME280_LEN_TEMP_PRESS_CALIB_DATA          26
#define BME280_LEN_HUMIDITY_CALIB_DATA            7
#define BME280_LEN_P_T_H_DATA                     8

/** @name Sensor component selection macros. Internal for API implementation. */
#define BME280_PRESS                              1
#define BME280_TEMP                               (1 << 1)
#define BME280_HUM                                (1 << 2)
#define BME280_ALL                                (0x07)

/** @name Macro to combine two 8 bit data's to form a 16 bit data */
#define BME280_CONCAT_BYTES(msb, lsb)             (((uint16_t)msb << 8) | (uint16_t)lsb)

/**
  BME280_INTF_RET_TYPE is the read/write interface return type which can be overwritten by the build system.
*/
#ifndef BME280_INTF_RET_TYPE
#define BME280_INTF_RET_TYPE                      int8_t
#endif

#define BME280_CS								CS3 

#pragma endregion Macros


#pragma region Pointers
/**
 @brief Bus communication function pointer which should be mapped to the specific read functions
 @param reg_addr Register address from which data is read
 @param reg_data Pointer to data buffer where read data is stored
 @param len Number of bytes of data to be read
*/
typedef BME280_INTF_RET_TYPE (*BME280_read_fptr_t)(uint8_t reg_addr, uint8_t *reg_data, uint32_t len);

/*!
 @brief Bus communication function pointer which should be mapped to the specific write functions
 @param reg_addr Register address to which the data is written
 @param reg_data Pointer to data buffer in which data to be written is stored
 @param len Number of bytes of data to be written
*/
typedef BME280_INTF_RET_TYPE (*BME280_write_fptr_t)(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len);


//SPI
SPI_TypeDef BME280_SPI;


#pragma region Structs/Emun

/**
  @brief Sensor structure that holds compensated temp, pres and humidity data
*/
typedef struct BME280_data
{
  uint32_t pressure;    // Compensated pressure
  int32_t temperature;  // Compensated temperature
  uint32_t humidity;    // Compensated humidity
} BME280_data;


/**
  @brief Sensor structure that holds uncompensated temp, pres and humidity data
*/
typedef struct BME280_uncomp_data
{
  uint32_t pressure;    // Un-compensated pressure
  uint32_t temperature; // Un-compensated temperature
  uint32_t humidity;    // Un-compensated humidity
} BME280_uncomp_data;


/**
  @brief Calibration data
*/
typedef struct BME280_calib_data
{
  // Calibration coefficient for the temperature sensor 
  uint16_t dig_t1;  
  int16_t dig_t2;
  int16_t dig_t3;

  // Calibration coefficient for the pressure sensor 
  uint16_t dig_p1;
  int16_t dig_p2;
  int16_t dig_p3;
  int16_t dig_p4;
  int16_t dig_p5;
  int16_t dig_p6;
  int16_t dig_p7;
  int16_t dig_p8;
  int16_t dig_p9;

  // Calibration coefficient for the humidity sensor 
  uint8_t dig_h1;
  int16_t dig_h2;
  uint8_t dig_h3;
  int16_t dig_h4;
  int16_t dig_h5;
  int8_t dig_h6;

  // Variable to store the intermediate temperature coefficient 
  int32_t t_fine;
} BME280_calib_data;

/**
 * @brief BME280 device structure
 */
typedef struct BME280_dev
{ 
    // Chip ID 
    uint8_t chip_ID;

    // Variable to store result of read/write function 
    BME280_INTF_RET_TYPE intf_rslt;

    // Read function pointer 
    BME280_read_fptr_t read;

    // Write function pointer 
    BME280_write_fptr_t write;

    // Trim data 
    BME280_calib_data calib;
} BME280_dev;

#pragma endregion Structs/Emun


#pragma region Functions
/**
  @brief Reads the chip-id of the sensor which is the first step to
 * verify the sensor and also calibrates the sensor
  @param dev Structure instance of BME280_dev
  @return Result of execution status
*/
int8_t BME280_init(BME280_dev *dev, SPI_TypeDef *spi);


/**
  @brief Soft-resets the sensor
  @param dev Structure instance of BME280_dev
  @return Result of execution status (same return as BME280_init) 
*/
int8_t BME280_soft_reset(BME280_dev *dev);


/**
  @brief Reads the data from the given register address of sensor
  @param reg_addr Register address from where the data to be read
  @param reg_data Pointer to data buffer to store the read data
  @param len Number of bytes of data to be read
  @param dev Structure instance of BME280_dev
  @return Result of execution status (same return as BME280_init)
*/
int8_t BME280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, BME280_dev *dev);


/**
  @brief Writes the given data to the register address of the sensor
  @param reg_addr Register addresses to where the data is to be written
  @param reg_data Pointer to data buffer which is to be written in the regAddr of sensor.
  @param len Number of bytes of data to write
  @param dev Structure instance of BME280_dev
  @return Result of execution status (same return as BME280_init)
 */
int8_t BME280_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint32_t len, BME280_dev *dev);


/**
  @brief Reads the pres, temp and humidity data from the sensor, compensates the data and store 
  it in the BME280_data structure instance passed by the user.
  @param sensor_comp : Variable which selects which data to be read
  @verbatim
  sensorComp |   Macros
  -----------|-----------------
    1        |  BME280_PRESS
    2        |  BME280_TEMP
    4        |  BME280_HUM
    7        |  BME280_ALL
  @endverbatim
  @param comp_data : Structure instance of BME280_data.
  @param dev      : Structure instance of BME280_dev.

  @return Result of execution status
*/
int8_t BME280_get_data(uint8_t sensor_comp, BME280_data *comp_data, BME280_dev *dev);


/**
  @brief Compensate the pres and/or temp and/or humidity data according to the selected component
  @param sensor_comp : Used to select pressure and/or temp and/or humidity
  @param uncomp_data : Contains the uncompensated pres, temp and humidity data
  @param comp_data   : Contains the compensated pres and/or temp and/or humidity data
  @param calib_data  : Pointer to BME280_calibData
  @return Result of execution status.
*/
int8_t BME280_compensate_data(uint8_t sensor_comp, const BME280_uncompData *uncomp_data,
                              BME280_data *comp_data, BME280_calibData *calib_data);

/**
 @brief This private function reads the calibration data from the sensor, parse
 it and store in the device structure.
*/
static int8_t get_calib_data(BME280_dev *dev);

#pragma endregion Functions

#endif /* BME280_DRIVER_H */
