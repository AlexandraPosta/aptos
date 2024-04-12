/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Alexandra Posta
    Created on: 08 March 2024
    Description: Buffer data for the sensors loop
*/

#include "data_buffer.h"

void init_buffer(dataBuffer* buffer) {
  buffer->ground_ref = 0;
  buffer->index = 0;
  buffer->count = 0;
}

/**
  @brief Compare function for sorting purposes
  @param a - first element
  @param b - second element
  @return comparison value
*/
int cmpfunc(const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int get_median(int data[], int size) {
  qsort(data, sizeof(data), sizeof(int), cmpfunc);
  if (sizeof(data) % 2 != 0)
    return data[(size / 2) + 1];
  return (data[size / 2] + data[(size / 2)+ 1]) / 2;
}

void set_ground_reference(dataBuffer* buffer) {
  // Create copy of buffer data to sort
  int _data[WINDOW_SIZE];
  for (int i = 0; i < WINDOW_SIZE; i++) {
    _data[i] = buffer->frames[i].barometer.pressure;
  }

  // get the ground reference as median
  buffer->ground_ref = get_median(_data, WINDOW_SIZE);
}

// Circular Buffer logic to update data
void update_buffer(FrameArray* frame, dataBuffer* buffer) {
  buffer->frames[buffer->index] = *frame;
  buffer->index = (buffer->index + 1) % BUFFER_SIZE;

  // Increase count and set ground reference
  if (buffer->count < BUFFER_SIZE) {
    if (buffer->count == WINDOW_SIZE) 
      set_ground_reference(buffer);
    buffer->count++;
  }

  // Update window
  if (buffer->count > WINDOW_SIZE*2) {
    for (int i = 0; i < WINDOW_SIZE; i++) {
      int frame_number = (buffer->index - WINDOW_SIZE + i);
      if (frame_number < 0)
        frame_number = BUFFER_SIZE + frame_number;
      buffer->window[i] = buffer->frames[i];
    }
  }
}

float get_vertical_velocity(int previous_barometer, int new_barometer, int size, int dt) {
  float altitude_change = 0.0;
  float previous_altitude, current_altitude;
  float total_time = dt * 1e-6; 

  //get first value
  previous_altitude = 44330.7692 * (1.0 - pow(previous_barometer / sea_level_pressure, 0.1902)); 
  current_altitude = 44330.7692 * (1.0 - pow(new_barometer / sea_level_pressure, 0.1902)); 
  altitude_change = current_altitude - previous_altitude;

  // Iterate over the barometer readings
  //for (int i = 1; i < size; ++i) {
  //    // Convert current reading to altitude
  //    current_altitude = 44330.7692 * (1.0 - pow(barometer_data[i] / sea_level_pressure, 0.1902));
  //    // Calculate the change in altitude
  //    altitude_change_sum += current_altitude - previous_altitude;
  //    previous_altitude = current_altitude;
  //}

  // Calculate the total time covered by the readings (microseconds)
  
  // Return vertical velocity in m/s
  return altitude_change / total_time;
}

// Check if stationary
bool is_stationary(int data[]) {
  int sum = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) {
    sum += data[i];
  }
  int mean = sum / WINDOW_SIZE;
  int variance = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) {
    variance += pow((data[i] - mean), 2);
  }
  return sqrt(variance / WINDOW_SIZE) < GROUND_THRESHOLD;
}
