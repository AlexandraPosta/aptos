/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Sam Bruton
    Created on: 8th April 2024
    Description: Combine Gyro Data with Accelerometer Data through utilisation of Kalman Filter
*/
#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "drivers/LSM6DS3_driver.h"
#include "drivers/ADXL375_driver.h"
#include <math.h>

#define M_PI_F 3.14159265358979323846f
#pragma region Structs/Emun
typedef struct ADXL375_data
{
  int16_t x;
  int16_t y;
  int16_t z;
} ADXL375_data;

#pragma endregion Structs/Emun

typedef struct Euler {
    float roll;
    float pitch; 
    float yaw;
} Euler;

typedef struct orientation_data {
    Quaternion current_quaternion;
    Quaternion current_rate_quaternion;
    Euler current_euler;
    Euler current_rate_euler;
    Euler previous_euler;
} orientation_data;

void kalman_init((orientation_data* data, ADXL375_data* data));
void kalman1D(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement);

#endif /* KALMAN_FILTER_H */