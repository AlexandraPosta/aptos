/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Description: Transform gyroscope data to quateniun and euler matrices
*/

#ifndef ORIENTATION_UTILS_H
#define ORIENTATION_UTILS_H

#include "drivers/LSM6DS3_driver.h"
#include <math.h>

#define M_PI_F 3.14159265358979323846f

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

/**
   @brief Convert euler angles to quaternion
   @param e Euler angles
   @param q Quaternion
*/
void orientation_quaternion_to_euler(Quaternion* q, Euler* e);

/**
  @brief Initialise the orientation data
  @note Set the orientation_data structure to 0 to initialise memory
*/
void orientation_init(orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data);

/**
  @brief Update the orientation data based on gyro readings
  @param dt Time difference in microseconds
  @param orientation Orientation data structure
  @param _LSM6DS3_data Gyroscope data
*/
void orientation_update(unsigned int dt, orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data, bool pad);

/**
  @brief Check if rocket is moving based on acceleration vector
  @param _LSM6DS3_data Gyroscope data
  @param vector Acceleration vector
  @return True if the vector is valid
*/
bool OrientationAccelerationVector(LSM6DS3_data* _LSM6DS3_data, float vector[]);

/**
  @brief Check if stationary, to correct gyro drift, based on acceleration vector
  @param _orientation Orientation data structure
  @param accel Acceleration vector
  @param correction Quaternion correction
*/
void OrientationAccelerationQuaternion(orientation_data* _orientation, float accel[], Quaternion* correction);
#endif /* ORIENTATION_UTILS_H */