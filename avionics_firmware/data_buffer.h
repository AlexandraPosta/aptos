/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Alexandra Posta
    Created on: 08 March 2024
    Description: Buffer data for the sensors loop
*/

#ifndef BUFFER_H
#define BUFFER_H

#include "drivers/ADXL375_driver.h"
#include "drivers/MS5611_driver.h"
#include <stdio.h>
#include <stdlib.h>
#include "mcu.h"

// Define Constants and Thresholds
#define BUFFER_SIZE       50
#define LAUNCH_THRESHOLD  50      // micro bar for detecting a decrease
#define APOGEE_THRESHOLD  50      // micro bar for detecting apogee
#define DESCENT_THRESHOLD 50      // micro bar for detecting an increase
#define GROUND_THRESHOLD  30      // micro bar for detecting ground
#define WINDOW_SIZE       10      // Number of readings to compute

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
#endif /* BUFFER_H */

