/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Description: Frame structure used for storing and reading off the NAND Flash
*/  	

#ifndef FRAME_ARRAY_H
#define FRAME_ARRAY_H

#include "mcu.h"
#include "orientation_utils.h"
#include "drivers/MS5611_driver.h"
#include "drivers/LSM6DS3_driver.h"
#include "drivers/ADXL375_driver.h"
#include "drivers/BME280_driver.h"

// Max 128 bytes
typedef struct FrameArray {
  DateTime date;            // 56 bits
  uint8_t changeFlag;       // 8 bits
  ADXL375_data accel;       // 48 bits
  LSM6DS3_data imu;         // 240 bits
  M5611_data barometer;     // 64 bits
  GNSS_Data GNSS;           // 64 bits
  BME280_data bme;          // 80 bits
  Euler euler;              // 96 bits
  Euler euler_rate;         // 96 bits
  Euler euler_kalman;       // 96 bits
  ServoDeflections servos;  // 64 bits
  uint8_t hammingCode[8];
  uint16_t CRC_Check;
  int successFlag;          // Not used in zip
} FrameArray;

#endif /* FRAME_ARRAY_H */