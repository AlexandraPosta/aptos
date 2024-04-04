/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 04 April 2024
  Description: Frame struture used for storing and reading off the NAND Flash
*/  	

#ifndef FRAME_ARRAY_H
#define FRAME_ARRAY_H

#include "mcu.h"
#include "orientation_utils.h"
#include "drivers/MS5611_driver.h"
#include "drivers/LSM6DS3_driver.h"
#include "drivers/ADXL375_driver.h"

// Max 128 bytes
typedef struct FrameArray {
  DateTime date;            // 80 bits
  uint16_t changeFlag;      // 16 bits
  ADXL375_data accel;       // 48 bits
  LSM6DS3_data imu;         // 368 bits
  M5611_data barometer;     // 64 bits
  GNSS_Data GNSS;           // 96 bits
  Euler euler;              // 96 bits
  Euler euler_rate;         // 96 bits
  ServoDeflections servos;  // 32 bits
  uint8_t hammingCode[8];
  uint16_t CRC_Check;
  int successFlag; // Not used in zip
} FrameArray;

#endif /* FRAME_ARRAY_H */