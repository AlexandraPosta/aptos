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

/**
  @brief Test Routine
*/
void run_test_routine2() ;

void run_test_routine() ;

/**
  @brief Test Routine
*/
void run_test_routine_MS5611();
/**
  @brief Test Routine
*/
void run_test_routine_BME280();

void run_test_routine_LSM6DS3();

void run_ADXL375_routine();

void ServoTest();

void timer_test();

/**
  @brief Routine to check if it should enter DFU boot mode.
*/
//void NAND_flash_test_routine();

void DFU_programming_check();



#endif /*TESTING_ROUTINE_H*/
