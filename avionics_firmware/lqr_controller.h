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

/**
  @brief Initialise the LQR controller
  @param lqr LQR controller structure
*/
void LQR_init(LQR_controller* lqr);

/**
  @brief Update the gains of the LQR controller
  @param lqr LQR controller structure
  @param velocity Current velocity of the rocket
  @note the gains are set to zero if the velocity is below or above a threshold
*/
void LQR_update_gain(LQR_controller* lqr, int velocity);

/**
  @brief Perform the LQR control
  @param lqr LQR controller structure
  @param orientation Current orientation data
  @param servo_defs Servo deflections angles
*/
void LQR_perform_control(LQR_controller* lqr, orientation_data orientation, char* servo_defs);

#endif /* LQR_CONTROLLER_DRIVER_H */
