/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Oliver Martin
  Created on: 01 March 2024
  Description: header file for the IMU module LSM6DS3
*/
#ifndef LSM6DS3_DRIVER_H
#define LSM6DS3_DRIVER_H
#include "mcu.h"
#include "math.h"
#include "filters.h"

#pragma once

#define LSM6DS3_CS      CS1
#define LSM6DSO_CHIP_ID 0x6C

// equivalent to 70 mdps/LSB, as specified in LSM6DSO datasheet section 4.1, symbol G_So
#define LSM6DSO_GYRO_SCALE_2000DPS 0.070f

#define LSM6DS3_WHO_AM_I_EXP 0b01101010

// LSM6DSO registers (not the complete list)

#define LSM6DSO_REG_INT1_CTRL 0x0D  // int pin 1 control
#define LSM6DSO_REG_INT2_CTRL 0x0E  // int pin 2 control
#define LSM6DSO_REG_WHO_AM_I 0x0F   // chip ID
#define LSM6DSO_REG_CTRL1_XL 0x10   // accelerometer control
#define LSM6DSO_REG_CTRL2_G 0x11    // gyro control
#define LSM6DSO_REG_CTRL3_C 0x12    // control register 3
#define LSM6DSO_REG_CTRL4_C 0x13    // control register 4
#define LSM6DSO_REG_CTRL5_C 0x14    // control register 5
#define LSM6DSO_REG_CTRL6_C 0x15    // control register 6
#define LSM6DSO_REG_CTRL7_G 0x16    // control register 7
#define LSM6DSO_REG_CTRL8_XL 0x17   // control register 8
#define LSM6DSO_REG_CTRL9_XL 0x18   // control register 9
#define LSM6DSO_REG_CTRL10_C 0x19   // control register 10
#define LSM6DSO_REG_STATUS 0x1E     // status register
#define LSM6DSO_REG_OUT_TEMP_L 0x20 // temperature LSB
#define LSM6DSO_REG_OUT_TEMP_H 0x21 // temperature MSB
#define LSM6DSO_REG_OUTX_L_G 0x22   // gyro X axis LSB
#define LSM6DSO_REG_OUTX_H_G 0x23   // gyro X axis MSB
#define LSM6DSO_REG_OUTY_L_G 0x24   // gyro Y axis LSB
#define LSM6DSO_REG_OUTY_H_G 0x25   // gyro Y axis MSB
#define LSM6DSO_REG_OUTZ_L_G 0x26   // gyro Z axis LSB
#define LSM6DSO_REG_OUTZ_H_G 0x27   // gyro Z axis MSB
#define LSM6DSO_REG_OUTX_L_A 0x28   // acc X axis LSB
#define LSM6DSO_REG_OUTX_H_A 0x29   // acc X axis MSB
#define LSM6DSO_REG_OUTY_L_A 0x2A   // acc Y axis LSB
#define LSM6DSO_REG_OUTY_H_A 0x2B   // acc Y axis MSB
#define LSM6DSO_REG_OUTZ_L_A 0x2C   // acc Z axis LSB
#define LSM6DSO_REG_OUTZ_H_A 0x2D   // acc Z axis MSB


// LSM6DSO register configuration values
#define LSM6DSO_VAL_INT1_CTRL 0x02             // enable gyro data ready interrupt pin 1
#define LSM6DSO_VAL_INT2_CTRL 0x02             // enable gyro data ready interrupt pin 2
#define LSM6DSO_VAL_CTRL1_XL_ODR833 0x07       // accelerometer 833hz output data rate (gyro/8)
#define LSM6DSO_VAL_CTRL1_XL_ODR1667 0x08      // accelerometer 1666hz output data rate (gyro/4)
#define LSM6DSO_VAL_CTRL1_XL_ODR3332 0x09      // accelerometer 3332hz output data rate (gyro/2)
#define LSM6DSO_VAL_CTRL1_XL_ODR3333 0x0A      // accelerometer 6664hz output data rate (gyro/1)
#define LSM6DSO_VAL_CTRL1_XL_8G 0x03           // accelerometer 8G scale
#define LSM6DSO_VAL_CTRL1_XL_16G 0x01          // accelerometer 16G scale
#define LSM6DSO_VAL_CTRL1_XL_LPF1 0x00         // accelerometer output from LPF1
#define LSM6DSO_VAL_CTRL1_XL_LPF2 0x01         // accelerometer output from LPF2
#define LSM6DSO_VAL_CTRL2_G_ODR6664 0x0A       // gyro 6664hz output data rate
#define LSM6DSO_VAL_CTRL2_G_ODR26 0x02         // gyro 26hz output data rate
#define LSM6DSO_VAL_CTRL2_G_2000DPS 0x03       // gyro 2000dps scale
#define LSM6DSO_VAL_CTRL2_G_245DPS 0x00       // gyro 245dps scale
#define LSM6DSO_VAL_CTRL3_C_BDU BIT(6)         // (bit 6) output registers are not updated until MSB and LSB have been read (prevents MSB from being updated while burst reading LSB/MSB)
#define LSM6DSO_VAL_CTRL3_C_H_LACTIVE 0        // (bit 5) interrupt pins active high
#define LSM6DSO_VAL_CTRL3_C_PP_OD 0            // (bit 4) interrupt pins push/pull
#define LSM6DSO_VAL_CTRL3_C_SIM 0              // (bit 3) SPI 4-wire interface mode
#define LSM6DSO_VAL_CTRL3_C_IF_INC BIT(2)      // (bit 2) auto-increment address for burst reads
#define LSM6DSO_VAL_CTRL4_C_I2C_DISABLE BIT(2) // (bit 2) disable I2C interface
#define LSM6DSO_VAL_CTRL4_C_LPF1_SEL_G BIT(1)  // (bit 1) enable gyro LPF1
#define LSM6DSO_VAL_CTRL6_C_XL_HM_MODE 0      // (bit 4) enable accelerometer high performance mode
#define LSM6DSO_VAL_CTRL6_C_FTYPE_335HZ 0x00   // (bits 2:0) gyro LPF1 cutoff 335.5hz
#define LSM6DSO_VAL_CTRL6_C_FTYPE_232HZ 0x01   // (bits 2:0) gyro LPF1 cutoff 232.0hz
#define LSM6DSO_VAL_CTRL6_C_FTYPE_171HZ 0x02   // (bits 2:0) gyro LPF1 cutoff 171.1hz
#define LSM6DSO_VAL_CTRL6_C_FTYPE_609HZ 0x03   // (bits 2:0) gyro LPF1 cutoff 609.0hz
#define LSM6DSO_VAL_CTRL9_XL_I3C_DISABLE BIT(1)// (bit 1) disable I3C interface



// LSM6DSO register configuration bit masks

#define LSM6DSO_MASK_CTRL3_C 0x7C         // 0b01111100
#define LSM6DSO_MASK_CTRL3_C_RESET BIT(0) // 0b00000001
#define LSM6DSO_MASK_CTRL4_C 0x06         // 0b00000110
#define LSM6DSO_MASK_CTRL6_C 0x17         // 0b00010111
#define LSM6DSO_MASK_CTRL9_XL 0x02        // 0b00000010

// Calibration parameters
#define LSM6DSO_OFFSET_BUFF_LEN 50
#define LSM6DS6_DOWNSAMPLE_SIZE 4
#define LMS6DS6_ANGULAR_RATE_SENSITIVITY  70 //for +-2000dps sensitivity is 70mdps/LSB
#define LMS6DS6_ACCEL_SENSITIVITY  488 //for +-16G, sensitivity is 0.488 mg/LSB

// Constants for filter tuning
#define GYRO_WEIGHT 100//0.98
#define ACCEL_WEIGHT 0//0.02

typedef struct LSM6DS3_data
{
  int32_t x;
  int32_t y;
  int32_t z;
  int32_t x_rate;
  int32_t y_rate;
  int32_t z_rate;
  int32_t x_offset;
  int32_t y_offset;
  int32_t z_offset;
  int16_t x_accel;
  int16_t y_accel;
  int16_t z_accel;
  int32_t time;
} LSM6DS3_data;

/**
	@brief Initialises the IMU
	@note sets registers and runs gyro offset calc
	@return 1 for success, 0 for wrong chip id
*/
uint8_t Lsm6ds3Init(SPI_TypeDef *spi, LSM6DS3_data* gyro);

static void Lsm6ds3WriteRegister(SPI_TypeDef *spi, uint8_t register_id, uint8_t value, unsigned delayMs);
static void Lsm6ds3WriteRegisterBits(SPI_TypeDef *spi, uint8_t register_id, uint8_t mask, uint8_t value, unsigned delayMs);

/**
	@brief Configures the settings registers for the IMU
	@note Sets things like frequency and range
*/
void Lsm6ds3Config(SPI_TypeDef *spi);

/**
	@brief Reads the raw Accel data
	@note results are stored in the gyro pointer
*/
bool Lsm6ds3AccRead(SPI_TypeDef *spi, LSM6DS3_data* gyro);

/**
	@brief Reads the raw Gyro data
	@note results are stored in the gyro pointer
*/
bool Lsm6ds3GyroRead(SPI_TypeDef *spi, LSM6DS3_data* gyro);

/**
	@brief Calculates the angles by integrating the raw gyro readings
	@note results are stored in the gyro pointer
*/
bool Lsm6ds3GyroReadAngle(SPI_TypeDef *spi, LSM6DS3_data* gyro);

/**
	@brief Calculates offsets to zero the gyro
	@note must be stationary while this is performed
*/
bool Lsm6ds3GyroOffsets(SPI_TypeDef *spi, LSM6DS3_data* gyro);

/**
	@brief Calculates if standard divation of readings is within a threshold limit
	@note Used to work out if the board is stationary enough to use offsets
  @returns true if within limits
*/
bool Lsmds3GyroStandardDev(LSM6DS3_data buff[], uint16_t limit);

/**
	@brief Stops angle from overflowing.
	@note keeps angle between +-180,000 mDeg
*/
int32_t Lsm6ds3AngleOverflow(int32_t mDeg);

#endif /* LSM6DS3_DRIVER_H */
