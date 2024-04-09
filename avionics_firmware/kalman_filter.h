/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Sam Bruton
    Created on: 8th April 2024
    Description: Combine Gyro Data with Accelerometer Data through utilisation of Kalman Filter
*/
#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

#include "drivers/LSM6DS3_driver.h"
#include "orientation_utils.h"

#pragma region Structs/Emun
typedef struct LSM6DS3_data
{
  int32_t x_rate;
  int32_t y_rate;
  int32_t z_rate;
  int32_t x_offset;
  int32_t y_offset;
  int32_t z_offset;
  int16_t x_accel;
  int16_t y_accel;
  int16_t z_accel;
} LSM6DS3_data;

#pragma endregion Structs/Emun

typedef struct Euler {
    float roll;
    float pitch; 
    float yaw;
} Euler;

typedef struct Quaternion {
    float w;
    float x;
    float y;
    float z;
} Quaternion;

typedef struct orientation_data {
    Quaternion current_quaternion;
    Quaternion current_rate_quaternion;
    Euler current_euler;
    Euler current_rate_euler;
    Euler previous_euler;
} orientation_data;

typedef struct kalman_data{
    Euler state;
    Euler gain;
    Euler uncertainty;
    float angle_restriction;
    float gain_restriction_high; 
    float gain_restriction_low;
}kalman_data;

void kalman_filterInit(orientation_data* data, ADXL375_data* data, kalman_data* kalman_data);

void kalman_filter(orientation_data* data, ADXL375_data* data);

float kalman1D(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement);

float kalmanAngleRestriction(float restriction_angle, float input_angle);
float kalmanGainRestriction(float restriction_gain_high, float restriction_gain_low, float input_gain);

#endif /* KALMAN_FILTER_H */