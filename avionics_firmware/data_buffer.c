#include "data_buffer.h"

void init_buffer(dataBuffer* buffer) {
  buffer->ground_ref = 0;
  buffer->index = 0;
  buffer->count = 0;

  // Allocate memory for the buffer array
  /*
  //Create a blank frame
  FrameArray blankFrame;                         // initialise the frameArray that keeps updating
  uint8_t dataArray[128];                   // dummy array to store the frame data
  _memset(dataArray, 0, sizeof(dataArray)); // set the necessary memory and set values to 0
  blankFrame = unzip(&dataArray);               // convert from normal array into FrameArray

  //add blank frames to buffers
  for (uint8_t i = 0; i < BUFFER_SIZE; i ++){
    buffer->frames[i] = blankFrame;
  }
  */
  // TODO: Allocate memory for the window array
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
    _data[i] = buffer->frames[i].barometer;
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

// Check if stationary
bool is_stationary(int data[]) {
  int sum = 0;
  for (int i = 0; i < WINDOW_SIZE; i++) {
    sum += data[i];
  }
  return (sum / WINDOW_SIZE) < GROUND_THRESHOLD;
}
