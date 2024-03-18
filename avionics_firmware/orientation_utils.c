/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Alexandra Posta
    Created on: 18 March 2024
    Description: Transform gyroscope data to quateniun and euler matrices
*/

#include "orientation_utils.h"

void orientation_quaternion_to_euler(Quaternion* q, Euler* e) {
    // XYZ order
    float qw2 = q->w * q->w;
    float qx2 = q->x * q->x;
    float qy2 = q->y * q->y;
    float qz2 = q->z * q->z;

    // Calculate direction cosine matrix
    float dcm32 = 2 * (q->y * q->z - q->x * q->w);
    float dcm33 = qw2 - qx2 - qy2 + qz2;
    float dcm31 = 2 * (q->x * q->z + q->y * q->w);
    float dcm21 = 2 * (q->x * q->y - q->z * q->w);
    float dcm11 = qw2 + qx2 - qy2 - qz2;

    // Calculate euler angles
    e->roll = (float)atan2(-dcm32, dcm33);
    e->pitch = (float)asin(dcm31);
    e->yaw = (float)atan2(-dcm21, dcm11);
}

void orientation_init(orientation_data* orientation) {
    // Set initial values for current_quaternion
    orientation->current_quaternion.w = 1;
    orientation->current_quaternion.x = 0;
    orientation->current_quaternion.y = 0;
    orientation->current_quaternion.z = 0;

    // Set initial values for current_rate_quaternion
    orientation->current_rate_quaternion.w = 0;
    orientation->current_rate_quaternion.x = 0;
    orientation->current_rate_quaternion.y = 0;
    orientation->current_rate_quaternion.z = 0;

    // Set initial values for current_euler
    orientation->current_euler.roll = 0;
    orientation->current_euler.pitch = 0;
    orientation->current_euler.yaw = 0;

    // Set initial values for current_rate_euler
    orientation->current_rate_euler.roll = 0;
    orientation->current_rate_euler.pitch = 0;
    orientation->current_rate_euler.yaw = 0;

    // Set initial values for previous_euler
    orientation->previous_euler.roll = 0;
    orientation->previous_euler.pitch = 0;
    orientation->previous_euler.yaw = 0;
}

void orientation_update(unsigned int dt, orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data) {
    float wx = (float)_LSM6DS3_data->x;
    float wy = (float)_LSM6DS3_data->y;
    float wz = (float)_LSM6DS3_data->z;
    float qw = orientation->current_quaternion.w;
    float qx = orientation->current_quaternion.x;
    float qy = orientation->current_quaternion.y;
    float qz = orientation->current_quaternion.z;

    // Calculate the derivative of the quaternion
    orientation->current_rate_quaternion.w = 0.5f * (-wx * qx - wy * qy - wz * qz);
    orientation->current_rate_quaternion.x = 0.5f * ( wx * qw + wz * qy - wy * qz);
    orientation->current_rate_quaternion.y = 0.5f * ( wy * qw - wz * qx + wx * qz);
    orientation->current_rate_quaternion.z = 0.5f * ( wz * qw + wy * qx - wx * qy);

    // Update quaternion using the derivative
    orientation->current_quaternion.w += orientation->current_rate_quaternion.w * (float)dt * 1e-6f;
    orientation->current_quaternion.x += orientation->current_rate_quaternion.x * (float)dt * 1e-6f;
    orientation->current_quaternion.y += orientation->current_rate_quaternion.y * (float)dt * 1e-6f;
    orientation->current_quaternion.z += orientation->current_rate_quaternion.z * (float)dt * 1e-6f;

    // Normalise quaternion
    float norm = sqrtf(orientation->current_quaternion.w * orientation->current_quaternion.w +
                       orientation->current_quaternion.x * orientation->current_quaternion.x +
                       orientation->current_quaternion.y * orientation->current_quaternion.y +
                       orientation->current_quaternion.z * orientation->current_quaternion.z);

    // Apply normalisation
    orientation->current_quaternion.w /= norm;
    orientation->current_quaternion.x /= norm;
    orientation->current_quaternion.y /= norm;
    orientation->current_quaternion.z /= norm;

    // Convert quaternion to euler angles
    orientation->previous_euler = orientation->current_euler;
    orientation_quaternion_to_euler(&orientation->current_quaternion, &orientation->current_euler);

    // Calculate the derivative of the euler angles
    if ((orientation->current_euler.roll < (M_PI_F - 0.6f)) && orientation->previous_euler.roll > (-M_PI_F + 0.6f)) {
        orientation->current_rate_euler.roll = 1e-6f * (orientation->current_rate_euler.roll + 2 * M_PI_F - orientation->previous_euler.roll) / (float)dt;
    } else {
        orientation->current_rate_euler.roll = 1e-6f * (orientation->current_rate_euler.roll - orientation->previous_euler.roll) / (float)dt;
    }
}