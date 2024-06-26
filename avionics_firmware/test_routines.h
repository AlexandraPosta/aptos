/*
	Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 03 March 2024
  Description: Header file for the drivers test routines.
*/
#ifndef TESTING_ROUTINE_H

#include "mcu.h"
#include "drivers/MS5611_driver.h"
#include "drivers/BME280_driver.h"
#include "drivers/ADXL375_driver.h"
#include "drivers/LSM6DS3_driver.h"
//#include "drivers/NAND_flash_driver.h"
#include "drivers/SERVO_driver.h"
#include "LQR_controller.h"
#include "orientation_utils.h"

/**
  @brief General Test Routine
*/
void run_test_routine2() ;

void run_test_routine() ;

/**
  @brief Test Routine for Barometer MS5611
*/
void run_test_routine_MS5611();

/**
  @brief Test Routine for Temperature BME280
*/
void run_test_routine_BME280();

/**
  @brief Test Routine for Gyroscope LSM6DS3
*/
void run_test_routine_LSM6DS3();

/**
  @brief Test Routine for Accelerometer ADXL375
*/
void run_ADXL375_routine();

/**
  @brief Test Routine for Servo
*/
void ServoTest();

/**
  @brief Test Routine for Timer
*/
void timer_test();

/**
  @brief Routine to check if it should enter DFU boot mode.
*/
void DFU_programming_check();

/**
  @brief Routine to check the LQR controller
*/
void run_controller_routine(LSM6DS3_data _LSM6DS3_data, orientation_data _orientation, LQR_controller _LQR_controller);

#endif /*TESTING_ROUTINE_H*/
