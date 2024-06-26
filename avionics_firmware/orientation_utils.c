/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Alexandra Posta
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

void orientation_change_accel_coordinate_system(LSM6DS3_data* _LSM6DS3_data) {
    int32_t temp_y = _LSM6DS3_data->y_accel;
    _LSM6DS3_data->y_accel = _LSM6DS3_data->z_accel;
    _LSM6DS3_data->z_accel = -temp_y;
}

void orientation_init(orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data) {
    float accel_vector[4];
    orientation_change_accel_coordinate_system(_LSM6DS3_data);
    if (OrientationAccelerationVector(_LSM6DS3_data, &accel_vector)) { //try to get an acceleration vector to use as starting angle
        float pitch_angle_accel = atan(accel_vector[1]/sqrt((accel_vector[0]*accel_vector[0])+(accel_vector[2]*accel_vector[2])));
        float yaw_angle_accel = atan(accel_vector[0]/sqrt((accel_vector[1]*accel_vector[1])+(accel_vector[2]*accel_vector[2])));

        // Calculate initial quaternion components based on the estimated roll and pitch angles
        float cy = cos(pitch_angle_accel * 0.5f);
        float sy = sin(pitch_angle_accel * 0.5f);
        float cp = cos(yaw_angle_accel * 0.5f);
        float sp = sin(yaw_angle_accel * 0.5f);
        orientation->current_quaternion.w = cp * cy;
        orientation->current_quaternion.x = sy * sp;
        orientation->current_quaternion.y = cp * sy;
        orientation->current_quaternion.z = sp * cy;

        orientation_quaternion_to_euler(&orientation->current_quaternion, &orientation->current_euler);
        // Set initial values for previous_euler
        orientation->previous_euler.roll = orientation->current_euler.roll;
        orientation->previous_euler.pitch = orientation->current_euler.pitch;
        orientation->previous_euler.yaw = orientation->current_euler.yaw;
    } else {  //accel wasn't close enough to 1g
        // Set initial values for current_quaternion
        orientation->current_quaternion.w = 1.0;
        orientation->current_quaternion.x = 0.0;
        orientation->current_quaternion.y = 0.0;
        orientation->current_quaternion.z = 0.0;
        // Set initial values for current_euler
        orientation->current_euler.roll = 0.0;
        orientation->current_euler.pitch = 0.0;
        orientation->current_euler.yaw = 0.0;
        // Set initial values for previous_euler
        orientation->previous_euler.roll = 0.0;
        orientation->previous_euler.pitch = 0.0;
        orientation->previous_euler.yaw = 0.0;
    }
    
    // Set initial values for current_rate_quaternion
    orientation->current_rate_quaternion.w = 0.0;
    orientation->current_rate_quaternion.x = 0.0;
    orientation->current_rate_quaternion.y = 0.0;
    orientation->current_rate_quaternion.z = 0.0;

    // Set initial values for current_rate_euler
    orientation->current_rate_euler.roll = 0.0;
    orientation->current_rate_euler.pitch = 0.0;
    orientation->current_rate_euler.yaw = 0.0;
}

void orientation_change_coordinate_system(LSM6DS3_data* _LSM6DS3_data) {
    int32_t temp_x = _LSM6DS3_data->x_rate;
    _LSM6DS3_data->x_rate = _LSM6DS3_data->y_rate;
    _LSM6DS3_data->y_rate = temp_x;
    _LSM6DS3_data->z_rate *= -1;
}

// Update orientation data
// On the sensor     -> X: PITCH, Y: ROLL,  Z:  YAW (right rule)
// On the controller -> X: ROLL,  Y: PITCH, Z: -YAW (left rule)
void orientation_update(unsigned int dt, orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data, bool pad) {
    // Change orientation data to match the controller coordinate system
    orientation_change_coordinate_system(_LSM6DS3_data);
    orientation_change_accel_coordinate_system(_LSM6DS3_data);
    float wx = ((float)_LSM6DS3_data->x_rate * M_PI_F / 180.0f) / 1000.0f; // millidegrees/second -> radians/second
    float wy = ((float)_LSM6DS3_data->y_rate * M_PI_F / 180.0f) / 1000.0f;
    float wz = ((float)_LSM6DS3_data->z_rate * M_PI_F / 180.0f) / 1000.0f;
    
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

    float accel_vector[4];
    if(OrientationAccelerationVector(_LSM6DS3_data, &accel_vector) && pad){ //try to get an acceleration vector to use as starting angle
        float pitch_angle_accel = atan(accel_vector[1]/sqrt((accel_vector[0]*accel_vector[0])+(accel_vector[2]*accel_vector[2])));
        float yaw_angle_accel = atan(accel_vector[0]/sqrt((accel_vector[1]*accel_vector[1])+(accel_vector[2]*accel_vector[2])));
        // Calculate initial quaternion components based on the estimated roll and pitch angles
        float cy = cos(pitch_angle_accel * 0.5f);
        float sy = sin(pitch_angle_accel * 0.5f);
        float cp = cos(yaw_angle_accel * 0.5f);
        float sp = sin(yaw_angle_accel * 0.5f);
        orientation->current_quaternion.w = 0.9f * orientation->current_quaternion.w + 0.1f * cp * cy;
        orientation->current_quaternion.x = 0.9f * orientation->current_quaternion.x + 0.1f * sy * sp;
        orientation->current_quaternion.y = 0.9f * orientation->current_quaternion.y + 0.1f * cp * sy;
        orientation->current_quaternion.z = 0.9f * orientation->current_quaternion.z + 0.1f * sp * cy;
    }

    // Normalise quaternions
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
    if ((orientation->current_euler.roll < (-(M_PI_F) + 0.6f)) && orientation->previous_euler.roll > (M_PI_F - 0.6f)) {
        orientation->current_rate_euler.roll = (orientation->current_euler.roll + 2 * M_PI_F - orientation->previous_euler.roll) / ((float)dt * 1e-6f);
    } else {
        orientation->current_rate_euler.roll = (orientation->current_euler.roll - orientation->previous_euler.roll) / ((float)dt * 1e-6f);
    }

    orientation->current_rate_euler.pitch = (orientation->current_euler.pitch - orientation->previous_euler.pitch) / ((float)dt* 1e-6f);
    orientation->current_rate_euler.yaw = (orientation->current_euler.yaw - orientation->previous_euler.yaw) / ((float)dt * 1e-6f);
}

bool OrientationAccelerationVector(LSM6DS3_data* _LSM6DS3_data, float vector[]){
    //convert from milli g to g
    vector[0] = _LSM6DS3_data->x_accel/1000.0;
    vector[1] = _LSM6DS3_data->y_accel/1000.0;
    vector[2] = _LSM6DS3_data->z_accel/1000.0;

    //check magnitude (in g)
    float magnitude = sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
    
    //normalise the vector
    vector[0] /= magnitude;
    vector[1] /= magnitude;
    vector[2] /= magnitude;
    vector[3] = magnitude;

    if (magnitude < 0.9 || magnitude > 1.1){   //if not close to 1G
        return false;
    }
    return true;
}

void OrientationAccelerationQuaternion(orientation_data* _orientation, float accel_vector[], Quaternion* correction){
    Quaternion q_est = _orientation->current_quaternion;
    
    // Estimate gravity direction in the world frame using current orientation estimate
    float gw_x = 2 * (q_est.x * q_est.z - q_est.w * q_est.y);
    float gw_y = 2 * (q_est.w * q_est.x + q_est.y * q_est.z);
    float gw_z = q_est.w * q_est.w - q_est.x * q_est.x - q_est.y * q_est.y + q_est.z * q_est.z;

    // Calculate error between estimated gravity direction and accelerometer readings
    float error_x = 2 * (accel_vector[0] * gw_x + accel_vector[1] * gw_y + accel_vector[2] * gw_z);
    float error_y = 2 * ((accel_vector[1] * gw_z - accel_vector[2] * gw_y));
    float error_z = 2 * ((accel_vector[2] * gw_x - accel_vector[0] * gw_z));

    // Compute feedback correction quaternion
    float alpha = 0.02f; // Correction gain
    correction->w = 1.0f;
    correction->x = alpha * error_x;
    correction->y = alpha * error_y;
    correction->z = alpha * error_z;
}