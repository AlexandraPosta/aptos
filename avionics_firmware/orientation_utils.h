/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Alexandra Posta
    Created on: 18 March 2024
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

void orientation_quaternion_to_euler(Quaternion* q, Euler* e);

void orientation_init();

void orientation_update(unsigned int dt, orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data);

bool OrientationAccelerationVector(LSM6DS3_data* _LSM6DS3_data, float vector[]);

#endif /* ORIENTATION_UTILS_H */