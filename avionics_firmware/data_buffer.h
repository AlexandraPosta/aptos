/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 08 March 2024
  Description: Buffer data for the sensors loop
*/

#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>

#include "drivers/ADXL375_driver.h"
#include "drivers/MS5611_driver.h"
#include "mcu.h"
#include "frame_array.h"

// Define Constants and Thresholds
#define BUFFER_SIZE       50
#define LAUNCH_THRESHOLD  50     // micro bar for detecting a decrease
#define APOGEE_THRESHOLD  50      // micro bar for detecting apogee
#define DESCENT_THRESHOLD 50      // micro bar for detecting an increase
#define GROUND_THRESHOLD  100     // micro bar for detecting ground
#define WINDOW_SIZE       20      // Number of readings to compute

static float sea_level_pressure = 1013.25;    // Sea level presser in micro bar

// Circular Buffer for data storing
typedef struct dataBuffer {
  FrameArray frames[BUFFER_SIZE]; // Circular buffer
  FrameArray window[WINDOW_SIZE]; // Last window readings
  int ground_ref;                 // Set of reference values for launch
  int index;                      // End index (value is inserted)
  int count;                      // Number of elements currently in buffer
} dataBuffer;

/**
  @brief Initialize the buffer
  @param buffer - data buffer
*/
void init_buffer(dataBuffer* buffer);

/**
  @brief Get the median of the data
  @param data - array of data
  @param size - size of the array
  @return median value
*/
int get_median(int data[], int size);

/**
  @brief Set the ground reference for the buffer
  @param frame - one reading data frame to add
  @param buffer - data buffer
*/
void update_buffer(FrameArray* frame, dataBuffer* buffer);

/**
  @brief Get the velocity from the buffer based on barometer readings
  @param barometer_data - array of barometer data in mbar
  @param size - size of the array
  @param dt - time interval between readings in microseconds
  @return float vertical velocity value in m/s
*/
int get_vertical_velocity(int data[], int size, int dt);

/**
  @brief Check if the rocket is stationary
  @param data - array of data
  @return true if the rocket is stationary
*/
bool is_stationary(int data[]);

#endif /* BUFFER_H */

