/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Sam Bruton
    Created on: 8th April 2024
    Description: Combine Gyro Data with Accelerometer Data through utilisation of Kalman Filter
*/

/*
Kalman Filter Operation Order:
1. Initialise Values to Start Values, Only do this the first time during setup.

Loop ->
2. Read Gyro and Accel Data
3. Update Kalman Roll
4. Update Kalman Pitch
5. Update Kalman Yaw
6. Log Kalman Values: Kalman Roll Angle, Kalman Pitch Angle, Kalman Roll Gain, Kalman Pitch Gain 
-> Loop end.
*/
#include "kalman_filter.h"

float kalman_filter_init(orientation_data* gyro_data, LSM6DS3_data* accel_data, kalman_data* kalman_data){
    //Kalman Output Angle Restriction:
    kalman_data->angle_restriction = 15;
    kalman_data->gain_restriction_high = 1;
    kalman_data->gain_restriction_low = 0;
 
    //THE BELOW VALUES SHOULD ONLY BE SET ON INIT OF FUNCTION
    //kalman1D Function Outputs Initialisation:
    //Roll:
    kalman_data->state.roll = 0;
    kalman_data->uncertainty.roll = 0.5*0.5;
    kalman_data->gain.roll = 0;

    //Pitch:
    kalman_data->state.pitch = 0;
    kalman_data->uncertainty.pitch = 0.5*0.5;
    kalman_data->gain.pitch = 0;
}

//Update Kalman Roll and Pitch Angles, Kalman Roll and Pitch Gains.
float kalman_filter(orientation_data* gyro_data, LSM6DS3_data* accel_data, kalman_data* kalman_data){
    //Gyro Rotation Rates:
    //float roll_rate = gyro_data->current_rate_euler.roll;
    //float pitch_rate = gyro_data->current_rate_euler.pitch; 
    float roll_angle_gyro = gyro_data->current_euler.roll;
    float pitch_angle_gyro = gyro_data->current_euler.pitch; 
    
    //Accelerometer Rotation Angles:
    //Add calibration values here:
    //This value is obtained by tilting the IMU in each axis direction and it is the difference between the value and 1.00.
    float acc_x_calibration = 0; 
    float acc_y_calibration = 0; 
    float acc_z_calibration = 0; 
    float acc_x = (accel_data->x)/(4096-acc_x_calibration);
    float acc_y = (accel_data->y)/(4096-acc_y_calibration);
    float acc_z = (accel_data->z)/(4096-acc_z_calibration);
    float roll_angle_accel = atan(acc_y/sqrt((acc_x*acc_x)+(acc_z*acc_z)))*(1/(3.142/180));
    float pitch_angle_accel = atan(acc_x/sqrt((acc_y*acc_y)+(acc_z*acc_z)))*(1/(3.142/180));
    printf_float(" Acc_x Value", acc_x);
    printf_float(" Acc_y Value", acc_y);
    printf_float(" Acc_z Value", acc_z);
    printf_float(" Accelerometer Roll Angle", roll_angle_accel);
    printf_float(" Accelerometer Roll Angle", pitch_angle_accel);
    printf_float(" Gyro Roll Angle", roll_angle_gyro);
    printf_float(" Gyro Pitch Angle", pitch_angle_gyro);
    printf("\r\n");
    //Gyro and Accel angles should be similar, with accel reacting to vibrations.

    float kalman_1d_output[] = {0,0,0};

    //Kalman Roll:
    kalman1D(kalman_data->state.roll, kalman_data->uncertainty.roll, roll_angle_gyro, roll_angle_accel, &kalman_1d_output);
    kalman_data->state.roll = kalmanAngleRestriction(kalman_data->angle_restriction, kalman_1d_output[0]);
    kalman_data->uncertainty.roll = kalman_1d_roll_output[1];
    //kalman_data->kalman_gain.roll = kalmanGainRestriction(kalman_restriction_gain_high, kalman_restriction_gain_low, kalman_1d_roll_output[2]);
    kalman_data->gain.roll  = kalman_1d_roll_output[2];


    //Kalman Pitch:
    kalman1D(kalman_data->state.pitch, kalman_data->uncertainty.pitch, pitch_angle_gyro, pitch_angle_accel, &kalman_1d_output);
    kalman_data->state.pitch = kalmanAngleRestriction(kalman_data->angle_restriction, kalman_1d_output[0]);
    kalman_data->uncertainty.pitch = kalman_1d_pitch_output[1];
    //kalman_data->kalman_gain.pitch = kalmanGainRestriction(kalman_restriction_gain_high, kalman_restriction_gain_low, kalman_1d_pitch_output[2]);
    kalman_data->gain.pitch  = kalman_1d_pitch_output[2];

    printf_float(" Kalman Roll Angle", kalman_data->kalman_state.roll);
    printf_float(" Kalman Roll Uncertainty", kalman_data->kalman_uncertainty.roll);
    printf_float(" Kalman Roll Gain", kalman_data->kalman_gain.roll);

    printf_float(" Kalman Pitch Angle", kalman_data->kalman_state.pitch);
    printf_float(" Kalman Pitch Uncertainty", kalman_data->kalman_uncertainty.pitch);
    printf_float(" Kalman Pitch Gain", kalman_data->kalman_gain.pitch );

    printf("\r\n");

};

//Kalman Filter Function:
float kalman1D(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement, float* kalman_1d_output){
    //kalman_state = angle calculated with the kalman filter
    //kalman_uncertainty = uncertainty of the state predicted by the kalman filter
    //kalman_input = rotation rate (gyro)
    //kalman_measurement = accelerometer angle
    
    //1. Predict current state of the system:
    kalman_state = kalman_state + 0.004*kalman_input;
    //2. Calculate the uncertainty of the prediction:
    kalman_uncertainty = kalman_uncertainty + (0.004*0.004*4*4);
    //3. Calculate the Kalman gain from the uncertanties of the prediciton and measurements:
    float kalman_gain = kalman_uncertainty * (1 / (1*kalman_uncertainty + (3*3)));
    //4. Update the predicted state of the system with the measurement of the state through the gain:
    kalman_state = kalman_state + kalman_gain*(kalman_measurement-kalman_state);
    //5. Update the uncertainty of the predicted state:
    kalman_uncertainty = (1-kalman_gain)*kalman_uncertainty;

    kalman_1d_output[0] = kalman_state;         //State Prediciton or Angle
    kalman_1d_output[1] = kalman_uncertainty;   //State Uncertainty
    kalman_1d_output[2] = kalman_gain;          //State Gain
}

//Function to Restrict the output angle of the kalman filter to a predetrimined angle.
float kalmanAngleRestriction(float restriction_angle, float input_angle){
    if (input_angle > restriction_angle){
        return restriction_angle;
    }
    else if (input_angle < -restriction_angle){
        return -restriction_angle;
    }
    else {
        return input_angle;
    }
}

//Not sure if this is required yet.
//Window Restriction of Kalman Gain output for recording.
float kalmanGainRestriction(float restriction_gain_high, float restriction_gain_low, float input_gain){
    if (input_gain > restriction_gain_high){
        return restriction_gain_high;
    }
    else if (input_gain < restriction_gain_low){
        return restriction_gain_low;
    }
    else {
        return input_gain;
    }
}