/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Sam Bruton
    Created on: 8th April 2024
    Description: Combine Gyro Data with Accelerometer Data through utilisation of Kalman Filter
*/
void kalman_filter(orientation_data* gyro_data, ADXL375_data* accel_data){
    //Gyro Rotation Rates:
    float roll_rate = current_rate_euler->roll;
    float pitch_rate = current_rate_euler->pitch; 
    float yaw_rate = current_rate_euler->yaw;
    //Gyro Rotation Rates Calibration Values:
    float roll_calibration_rate;
    float pitch_calibration_rate;
    float yaw_calibration_rate;
 
    //Gyro Rotation Angle:
    float roll_angle = current_euler->roll;
    float pitch_angle = current_euler->pitch;
    float yaw_angle = current_euler->yaw;

    //Accelerometer Rotation Angles:
    //Add calibration values here:
    float acc_x_calibration = 0; 
    float acc_y_calibration = 0; 
    float acc_z_calibration = 0; 
    int16_t acc_x = (accel_data->x)/(4096-acc_x_calibration);
    int16_t acc_y = (accel_data->y)/(4096-acc_y_calibration);
    int16_t acc_z = (accel_data->z)/(4096-acc_z_calibration);

    //kalman1D Function Outputs Initialisation:
    float kalman_roll_angle = 0;
    float kalman_roll_angle_uncertainty = 0.5*0.5;
    float kalman_pitch_angle = 0;
    float kalman_pitch_angle_uncertainty = 0.5*0.5;
    float kalman_yaw_angle = 0;
    float kalman_yaw_angle_uncertainty = 0.5*0.5;

    float kalman_1d_output[] = {0,0};

    //Kalman Roll:
    kalman1D(kalman_roll_angle, kalman_roll_angle_uncertainty, roll_rate, acc_x);
    kalman_roll_angle = kalman_1d_output[0];
    kalman_roll_angle_uncertainty = kalman_1d_output[1];

    //Kalman Pitch:
    kalman1D(kalman_pitch_angle, kalman_pitch_angle_uncertainty, pitch_rate, acc_y);
    kalman_pitch_angle = kalman_1d_output[0];
    kalman_pitch_angle_uncertainty = kalman_1d_output[1];

    //Kalman Yaw:
    kalman1D(kalman_yaw_angle, kalman_yaw_angle_uncertainty, yaw_rate, acc_z);
    kalman_yaw_angle = kalman_1d_output[0];
    kalman_yaw_angle_uncertainty = kalman_1d_output[1];

    printf_float(" Kalman Roll Angle", kalman_roll_angle);
    printf_float(" Kalman Roll Uncertainty", kalman_roll_angle_uncertainty);
    printf_float(" Kalman Pitch Angle", kalman_pitch_angle);
    printf_float(" Kalman Pitch Uncertainty", kalman_pitch_angle_uncertainty);
    printf_float(" Kalman Yaw Angle", kalman_yaw_angle);
    printf_float(" Kalman Yaw Uncertainty", kalman_yaw_angle_uncertainty);
    printf("\r\n");
};

void kalman1D(float kalman_state, float kalman_uncertainty, float kalman_input, float kalman_measurement){
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
}