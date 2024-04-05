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

void orientation_init(orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data) {
    float accel_vector[3];
    if(OrientationAccelerationVector(_LSM6DS3_data, &accel_vector)){ //try to get an acceleration vector to use as starting angle
        // Set initial values for current_quaternion
        /*
        orientation->current_quaternion.w = sqrtf(0.5f * (1 + accel_vector[2]));;
        orientation->current_quaternion.x = accel_vector[1] * sqrtf(0.5f * (1 - accel_vector[2])); // Handle sign ambiguity;
        orientation->current_quaternion.y = accel_vector[0] * sqrtf(0.5f * (1 - accel_vector[2])); // Handle sign ambiguity;
        orientation->current_quaternion.z = 0.0f; // No assumption about device's initial orientation
        */
        // Estimate roll and pitch angles
        float pitch = atan2(accel_vector[1], accel_vector[2]);
        float roll = atan2(-accel_vector[0], sqrt(accel_vector[1] * accel_vector[1] + accel_vector[2] * accel_vector[2]));
        // Calculate initial quaternion components based on the estimated roll and pitch angles
        float cy = cos(roll * 0.5f);
        float sy = sin(roll * 0.5f);
        float cp = cos(pitch * 0.5f);
        float sp = sin(pitch * 0.5f);
        orientation->current_quaternion.w = cy * cp;
        orientation->current_quaternion.x = cy * sp;
        orientation->current_quaternion.y = sy * cp;
        orientation->current_quaternion.z = -sy * sp;

        orientation_quaternion_to_euler(&orientation->current_quaternion, &orientation->current_euler);
        // Set initial values for previous_euler
        orientation->previous_euler.roll = orientation->current_euler.roll;
        orientation->previous_euler.pitch = orientation->current_euler.pitch;
        orientation->previous_euler.yaw = orientation->current_euler.yaw;
        printf_float("Initial quaternion, w:", orientation->current_quaternion.w, false);
        printf_float(", x: ", orientation->current_quaternion.x, false);
        printf_float(", y: ", orientation->current_quaternion.y, false);
        printf_float(", z: ", orientation->current_quaternion.z, false);
        printf("\r\n");
        printf_float("Initial euler, p:", orientation->current_euler.pitch/M_PI_F*180, false);
        printf_float(", r: ", orientation->current_euler.roll/M_PI_F*180, false);
        printf_float(", y: ", orientation->current_euler.yaw/M_PI_F*180, false);
        printf("\r\n");
    }else{  //accel wasn't close enough to 1g
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
    _LSM6DS3_data->x_rate = _LSM6DS3_data->z_rate;
    _LSM6DS3_data->z_rate = -temp_x;
}

void orientation_update(unsigned int dt, orientation_data* orientation, LSM6DS3_data* _LSM6DS3_data) {
    
    //assign measured sensor rotations
    float wx = (float)_LSM6DS3_data->x_rate*M_PI_F / 180 /1000.0; //convert from milli degrees to radians
    float wy = (float)_LSM6DS3_data->y_rate*M_PI_F / 180 /1000.0;
    float wz = (float)_LSM6DS3_data->z_rate*M_PI_F / 180 /1000.0;
    float qw = orientation->current_quaternion.w;
    float qx = orientation->current_quaternion.x;
    float qy = orientation->current_quaternion.y;
    float qz = orientation->current_quaternion.z;

    // Calculate the derivative of the quaternion
    orientation->current_rate_quaternion.w = 0.5f * (-wx * qx - wy * qy - wz * qz);
    orientation->current_rate_quaternion.x = 0.5f * ( wx * qw + wz * qy - wy * qz);
    orientation->current_rate_quaternion.y = 0.5f * ( wy * qw - wz * qx + wx * qz);	
    orientation->current_rate_quaternion.z = 0.5f * ( wz * qw + wy * qx - wx * qy);

    float accel_vector[3];
    if(OrientationAccelerationVector(_LSM6DS3_data, &accel_vector) && false){ //check if accceleration can be used to assist
        //use acceleration vector to help.
        // Update quaternion using the derivative & acceleration
        printf("Using acceleration\r\n");
        float acceleration_error[3];
        OrientationAccelerationQuaternion(orientation, &accel_vector, &acceleration_error);
        orientation->current_quaternion.w += orientation->current_rate_quaternion.w * (float)dt * 1e-6f;
        orientation->current_quaternion.x += (orientation->current_rate_quaternion.x * (float)dt * 1e-6f * 0.5) + (acceleration_error[0] * 0.02);
        orientation->current_quaternion.y += (orientation->current_rate_quaternion.y * (float)dt * 1e-6f * 0.5) + (acceleration_error[1] * 0.02);
        orientation->current_quaternion.z += (orientation->current_rate_quaternion.z * (float)dt * 1e-6f * 0.5) + (acceleration_error[2] * 0.02);
    }else{
        // Update quaternion using the derivative
        orientation->current_quaternion.w += orientation->current_rate_quaternion.w * (float)dt * 1e-6f;
        orientation->current_quaternion.x += orientation->current_rate_quaternion.x * (float)dt * 1e-6f;
        orientation->current_quaternion.y += orientation->current_rate_quaternion.y * (float)dt * 1e-6f;
        orientation->current_quaternion.z += orientation->current_rate_quaternion.z * (float)dt * 1e-6f;
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

    /*
    printf_float(" Roll", orientation->current_euler.roll, false);
    printf_float(" Pitch", orientation->current_euler.pitch, false);
    printf_float(" Yaw", orientation->current_euler.yaw, false);
    printf("\r\n");
    */

    // Calculate the derivative of the euler angles
    if ((orientation->current_euler.roll < (M_PI_F - 0.6f)) && orientation->previous_euler.roll > (-M_PI_F + 0.6f)) {
        orientation->current_rate_euler.roll = 1e-6f * (orientation->current_rate_euler.roll + 2 * M_PI_F - orientation->previous_euler.roll) / (float)dt;
    } else {
        orientation->current_rate_euler.roll = 1e-6f * (orientation->current_rate_euler.roll - orientation->previous_euler.roll) / (float)dt;
    }
}

bool OrientationAccelerationVector(LSM6DS3_data* _LSM6DS3_data, float vector[]){
    //convert from milli g to g
    vector[0] = _LSM6DS3_data->x_accel/1000.0;
    vector[1] = _LSM6DS3_data->y_accel/1000.0;
    vector[2] = _LSM6DS3_data->z_accel/1000.0;

    //check magnitude (in g)
    float magnitude = sqrtf(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
    if (magnitude < 0.9 || magnitude > 1.1){   //if not close to 1G
        return false;
    }
    //normalise the vector
    vector[0] /= magnitude;
    vector[1] /= magnitude;
    vector[2] /= magnitude;

    return true;
}

void OrientationAccelerationQuaternion(orientation_data* _orientation, float accel[], float e[]){
    float w = _orientation->current_quaternion.w;
    float x = _orientation->current_quaternion.x;
    float y = _orientation->current_quaternion.y;
    float z = _orientation->current_quaternion.z;
    //calculate cross product
    float ww = w * w;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float yy = y * y;
    float yz = y * z;
    float zz = z * z;

    //
    float rMat[3];
    rMat[0] = 2.0f * (xz + -wy);
    rMat[1] = 2.0f * (yz - -wx);
    rMat[2] = 1.0f - 2.0f * xx - 2.0f * yy;
    //
    e[0] = (accel[1] * rMat[2] - accel[2] * rMat[1]);
    e[1] = (accel[2] * rMat[0] - accel[0] * rMat[2]);
    e[2] = (accel[0] * rMat[1] - accel[1] * rMat[0]);
}