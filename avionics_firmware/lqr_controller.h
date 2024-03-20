/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 19 March 2024
  Description: Include LQR Controller header file
*/
#ifndef LQR_CONTROLLER_DRIVER_H
#define LQR_CONTROLLER_DRIVER_H

#include "orientation_utils.h"

#define STATE_SPACE_DIM     6   // Euler 3xangle 3xrates
#define NUM_GAINS           50
#define NUM_SERVOS          4
#define MAX_VELOCITY        120 // ms-1
#define MIN_VELOCITY        30  // ms-1

typedef struct LQR_controller {
    float* current_gain;
    float current_gain_index;
    float gain[NUM_GAINS * STATE_SPACE_DIM * NUM_SERVOS];
    float available_gains[NUM_GAINS * NUM_SERVOS * STATE_SPACE_DIM];
    float avg_gains[NUM_GAINS][NUM_SERVOS][STATE_SPACE_DIM];
    float zero_gains[NUM_SERVOS * STATE_SPACE_DIM];
} LQR_controller;

void LQR_init(LQR_controller* lqr);

void LQR_update_gain(LQR_controller* lqr, int velocity);

void LQR_perform_control(LQR_controller* lqr, orientation_data orientation, float* servo_defs[NUM_SERVOS]);

#endif /* LQR_CONTROLLER_DRIVER_H */
