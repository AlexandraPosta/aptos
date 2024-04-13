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
#include "mcu.h"
#include "drivers/LSM6DS3_driver.h"
#include "drivers/MS5611_driver.h"
#include <math.h>

typedef struct Axis {
    float x;
    float y;
    float z;
} Axis;

typedef struct vertical_velocity {
    float accel;
    float barom;
} vertical_velocity;

typedef struct kalman_velocity{
    float state;
    float uncertainty;
    float gain;
}kalman_velocity;

typedef struct kalman_data{
    Euler state;
    Euler uncertainty;
    Euler gain;
    float angle_restriction;
    float gain_restriction_high; 
    float gain_restriction_low;
    Axis accel_calibration;
    float accel_z_inertial;
    float pressure;
    float altitude;
    float altitude_init;
    float altitude_change;
    float altitude_previous;
    vertical_velocity velocity_measurement;
    kalman_velocity velocity;
} kalman_data;

void kalmanFilterInit(M5611_data* barometer_data, kalman_data* kalman_data);

void printDataForCollection(uint32_t current_time, float accel_x, float accel_y, float accel_z, float roll_angle_accel, float pitch_angle_accel, float yaw_angle_accel, float roll_angle_gyro, float pitch_angle_gyro, float yaw_angle_gyro, kalman_data* kalman_data);
static inline void printCSVHeaderKalman();

void kalmanFilterUpdate(orientation_data* gyro_data, LSM6DS3_data* accel_data, M5611_data* barometer_data ,kalman_data* kalman_data, int dt, float vertical_velocity_barom);

void kalmanFilter(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement, float* kalman_output);

float kalmanAngleRestriction(float restriction_angle, float input_angle);
float kalmanGainRestriction(float restriction_gain_high, float restriction_gain_low, float input_gain);

#endif /* KALMAN_FILTER_H */