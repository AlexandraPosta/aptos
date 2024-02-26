/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 01 February 2024
  Description: header file for the IMU module LSM6DS3
*/
#ifndef LSM6DS3_DRIVER_H
#define LSM6DS3_DRIVER_H
#include "mcu.h"

#pragma region Macros //_______________ MACROS _______________________
//register addresses
#define LSM6DS3_WHO_AM_I_ADRS                0x0F

#define LSM6DS3_CTRL1_XL_ADRS                0x10
#define LSM6DS3_CTRL2_G_ADRS                 0x11

#define LSM6DS3_TEMP_ADDRESS                 0x20

//chip settings
#define LSM6DS3_WHO_AM_I_EXP                 0b01101001 //expected val if who reg is read
#define LSM6DS3_CTRL1_XL_ON                  0b01000000
#define LSM6DS3_CTRL2_G_ON                   0b01000000

#define LSM6DS3_CS                           7

#pragma endregion Macros

#pragma region Structs/Emun //_______________ STRUCTS _______________________

typedef struct LSM6DS3_DATA
{
  int16_t LSM6DS3_TEMP;

  int16_t LSM6DS3_GYRO_X;
  int16_t LSM6DS3_GYRO_Y;
  int16_t LSM6DS3_GYRO_Z; 

  int16_t LSM6DS3_ACC_X;
  int16_t LSM6DS3_ACC_Y;
  int16_t LSM6DS3_ACC_Z;

} LSM6DS3_DATA;

#pragma endregion Structs/Emun

#pragma region Public

/**
  @brief Initialization of the LSM6DS3 IMU module
  @note
  @return Success/Failure
*/
int8_t LSM6DS3_init();

/**
 * @brief Get data from the LSM6DS3 IMU module
 * @note
 * @param data ptr to LSM6DS3_data struct for returning data
 * @return Success/Failure
 */
int8_t LSM6DS3_get_data(LSM6DS3_DATA *data);

#pragma endregion Public

#pragma region Private
/**
 * @brief write to a specific register
 * @note
 * @param address the register to write to
 * @param data data to send
 * @return Success/Failure
 */
static uint8_t LSM6DS3_write_reg(uint8_t address, uint8_t data);

/**
 * @brief Get a value from a register
 * @note
 * @param address the register to write to
 * @return the value read
 */
static uint8_t LSM6DS3_read_reg(uint8_t address);

/**
 * @brief extract and combine results from sensor registors
 * @note  doesnt deal with CS - purely for public get_data function
 * @return Success/Failure
 */
static int16_t LSM6DS3_get_sens_val();

#pragma endregion Private

#endif /* LSM6DS3_DRIVER_H */
