/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Sam Bruton
    Created on: 8th April 2024
    Description: Combine Gyro Data with Accelerometer Data through Utilisation of Kalman Filter
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

//Initialise the kalman_data struct with start data:
void kalmanFilterInit (kalman_data* kalman_data){
    //Kalman Output Angle Restriction:
    kalman_data->angle_restriction = 15;
    kalman_data->gain_restriction_high = 1;
    kalman_data->gain_restriction_low = 0;

    //Add calibration values here:
    //This value is obtained by tilting the IMU in each axis direction and it is the difference between the value and 1.00.
    kalman_data->accel_calibration.x = 0; 
    kalman_data->accel_calibration.y = 0; 
    kalman_data->accel_calibration.z = 0; 
 
    //THE BELOW VALUES SHOULD ONLY BE SET ON INIT OF FUNCTION
    //kalman1D Function Outputs Initialisation:
    //Roll:
    kalman_data->state.roll = 0;
    kalman_data->uncertainty.roll = 2*2;
    kalman_data->gain.roll = 0;

    //Pitch:
    kalman_data->state.pitch = 0;
    kalman_data->uncertainty.pitch = 2*2;
    kalman_data->gain.pitch = 0;

    //Yaw:
    kalman_data->state.yaw = 0;
    kalman_data->uncertainty.yaw = 2*2;
    kalman_data->gain.yaw = 0;

    printCSVHeaderKalman();
}

//Update Kalman Roll and Pitch Angles, Kalman Roll and Pitch Gains.
void kalmanFilterUpdate(orientation_data* gyro_data, LSM6DS3_data* accel_data, kalman_data* kalman_data){
    //Gyro Rotation Rates:
    float roll_rate_gyro = gyro_data->current_rate_euler.roll;
    float pitch_rate_gyro = -gyro_data->current_rate_euler.pitch; 
    float yaw_rate_gyr0 = -gyro_data->current_rate_euler.yaw;

    //Gyro Rotation Angles:
    float roll_angle_gyro = (gyro_data->current_euler.roll)*(1/(3.142/180));
    float pitch_angle_gyro = -(gyro_data->current_euler.pitch)*(1/(3.142/180)); 
    float yaw_angle_gyro = -(gyro_data->current_euler.pitch)*(1/(4.142/180));
    
    //Accelerometer Rotation Angles:
    //Convert From Milli G:
    float accel_x = (accel_data->x_accel)/1000.0f;
    float accel_y = (accel_data->y_accel)/1000.0f;
    float accel_z = (accel_data->z_accel)/1000.0f;
    //Calculate Angle Using Trig Method:
    //Note: Trig Method produces noisy signal.
    float roll_angle_accel = atan(accel_y/sqrt((accel_x*accel_x)+(accel_z*accel_z)))*(1/(3.142/180));
    float pitch_angle_accel = atan(accel_x/sqrt((accel_y*accel_y)+(accel_z*accel_z)))*(1/(3.142/180));
    float yaw_angle_accel = atan((sqrt((accel_x*accel_x)+(accel_y*accel_y)))/accel_z)*(1/(3.142/180));
    //Gyro and Accel angles should be similar, with accel reacting to vibrations.

    //Output from the kalmanFilter function, this gets over written every time the function is called:
    float kalman_output_roll[] = {0,0,0};
    float kalman_output_pitch[] = {0,0,0};
    float kalman_output_yaw[] = {0,0,0};
    //This has been seperated in to individual output vectors so that there is no discrete overlap or error when writing/readings values.

    //Kalman Roll:
    kalmanFilter(kalman_data->state.roll, kalman_data->uncertainty.roll, roll_angle_gyro, roll_angle_accel, &kalman_output_roll);
    //kalman_data->state.roll = kalmanAngleRestriction(kalman_data->angle_restriction, kalman_output[0]);
    kalman_data->state.roll = kalman_output_roll[0];
    kalman_data->uncertainty.roll = kalman_output_roll[1];
    //kalman_data->kalman_gain.roll = kalmanGainRestriction(kalman_restriction_gain_high, kalman_restriction_gain_low, kalman_1d_roll_output[2]);
    kalman_data->gain.roll  = kalman_output_roll[2];

    //Kalman Pitch:
    kalmanFilter(kalman_data->state.pitch, kalman_data->uncertainty.pitch, pitch_angle_gyro, pitch_angle_accel, &kalman_output_pitch);
    //kalman_data->state.pitch = kalmanAngleRestriction(kalman_data->angle_restriction, kalman_output[0]);
    kalman_data->state.pitch = kalman_output_pitch[0];
    kalman_data->uncertainty.pitch = kalman_output_pitch[1];
    //kalman_data->kalman_gain.pitch = kalmanGainRestriction(kalman_restriction_gain_high, kalman_restriction_gain_low, kalman_1d_pitch_output[2]);
    kalman_data->gain.pitch  = kalman_output_pitch[2];

    //Kalman Yaw:
    kalmanFilter(kalman_data->state.yaw, kalman_data->uncertainty.yaw, yaw_angle_gyro, yaw_angle_accel, &kalman_output_yaw);
    //kalman_data->state.yaw = kalmanAngleRestriction(kalman_data->angle_restriction, kalman_output[0]);
    kalman_data->state.yaw = kalman_output_yaw[0];
    kalman_data->uncertainty.yaw = kalman_output_yaw[1];
    //kalman_data->kalman_gain.yaw = kalmanGainRestriction(kalman_restriction_gain_high, kalman_restriction_gain_low, kalman_1d_pitch_output[2]);
    kalman_data->gain.yaw  = kalman_output_yaw[2];

    //Print for .csv file/dataset.
    //Get Current Time for Data Entry
    uint32_t current_time = get_time_ms();
    printf(", %i", current_time);
    //Acceleration in each axis:
    printf_float(",", accel_x, true);
    printf_float(",", accel_y, true);
    printf_float(",", accel_z, true);
    //Calculated Accelerometer angles:
    printf_float(",", roll_angle_accel, true);
    printf_float(",", pitch_angle_accel, true);
    printf_float(",", yaw_angle_accel, true);
    //Gyroscope angles:
    printf_float(",", roll_angle_gyro, true);
    printf_float(",", pitch_angle_gyro, true);
    printf_float(",", yaw_angle_gyro, true);
    //Gyro Rates:
    rintf_float(",", roll_angle_gyro, true);
    printf_float(",", pitch_angle_gyro, true);
    printf_float(",", yaw_angle_gyro, true);
    //Kalman Roll:
    printf_float(",", kalman_data->state.roll, true);
    printf_float(",", kalman_data->uncertainty.roll, true);
    printf_float(",", kalman_data->gain.roll, true);
    //Kalman Pitch:
    printf_float(",", kalman_data->state.pitch, true);
    printf_float(",", kalman_data->uncertainty.pitch, true);
    printf_float(",", kalman_data->gain.pitch, true);
    //Kalman Yaw:
    printf_float(",", kalman_data->state.yaw, true);
    printf_float(",", kalman_data->uncertainty.yaw, true);
    printf_float(",", kalman_data->gain.yaw, true);
    //New Line:
    printf("\r\n");

};

//Kalman Filter Function:
float kalmanFilter(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement, float* kalman_output){
    //kalman_state = angle calculated with the kalman filter
    //kalman_uncertainty = uncertainty of the state predicted by the kalman filter
    //kalman_input = rotation rate (gyro)
    //kalman_measurement = accelerometer angle
    //Control Matrix Value:
    float control_matrix = 0.004;
    //Standard Deviation of Gyro/Accel Values:
    float gyro_angle_stddev = 10;
    float accel_angle_stddev = 8;

    //1. Predict current state of the system:
    kalman_state = kalman_state + (control_matrix*kalman_input);
    //2. Calculate the uncertainty of the prediction:
    kalman_uncertainty = kalman_uncertainty + ((control_matrix*control_matrix)*(gyro_angle_stddev*gyro_angle_stddev));
    //3. Calculate the Kalman gain from the uncertanties of the prediciton and measurements:
    float kalman_gain = kalman_uncertainty * (1 / (1*kalman_uncertainty + (accel_angle_stddev*accel_angle_stddev)));
    //4. Update the predicted state of the system with the measurement of the state through the gain:
    kalman_state = kalman_state + (kalman_gain*(kalman_measurement-kalman_state));
    //5. Update the uncertainty of the predicted state:
    kalman_uncertainty = (1-kalman_gain)*kalman_uncertainty;

    kalman_output[0] = kalman_state;         //State Prediciton or Angle
    kalman_output[1] = kalman_uncertainty;   //State Uncertainty
    kalman_output[2] = kalman_gain;          //State Gain
}

//Function to Restrict the output angle of the kalman filter to a predetrimined angle range.
float kalmanAngleRestriction(float restriction_angle, float input_angle){
    //If the input angle is greater than the pos restriction angle then this is error so pass out restriction angle.
    if (input_angle > restriction_angle){
        return restriction_angle;
    }
    //If the input angle is less than the neg restriction angle then this is error so pass out restriction angle.
    else if (input_angle < -restriction_angle){
        return -restriction_angle;
    }
    //If the input angle is within the notch/window then output the input angle.
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

//Print Headers for .csv file:
static inline void printCSVHeaderKalman() {
    //Time:
    printf(",Time");
    //Axis G:
    printf(", Accelerometer X Value");
    printf(", Accelermoeter Y Value");
    printf(", Accelerometer Z Value");
    //Accel Angles:
    printf(", Accelerometer Roll Angle");
    printf(", Accelerometer Picth Angle");
    printf(", Accelerometer Yaw Angle");
    //Gyro Angles:
    printf(", Gyro Roll Angle");
    printf(", Gyro Pitch Angle");
    printf(", Gyro Yaw Angle");
    //Gyro Rates:
    printf(", Gyro Roll Rate");
    printf(", Gyro Pitch Rate");
    printf(", Gyro Yaw Rate");
    //Kalman Roll:
    printf(", Kalman Roll Angle");
    printf(", Kalman Roll Uncertainty");
    printf(", Kalman Roll Gain");
    //Kalman Pitch:
    printf(", Kalman Pitch Angle");
    printf(", Kalman Pitch Uncertainty");
    printf(", Kalman Pitch Gain");
    //Kalman Yaw:
    printf(", Kalman Yaw Angle");
    printf(", Kalman Yaw Uncertainty");
    printf(", Kalman Yaw Gain");
    //New Line:
    printf("\r\n");
}