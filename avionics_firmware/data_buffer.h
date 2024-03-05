#pragma once

#pragma region Events-Claculations

// Define Constants and Thresholds
#define BUFFER_SIZE       50
#define LAUNCH_THRESHOLD  50      // mbar for detecting a decrease
#define APOGEE_THRESHOLD  50      // mbar for detecting an increase
#define WINDOW_SIZE       10      // Number of readings to compute

// Buffer for data storing
typedef struct dataBuffer {
  int readings[BUFFER_SIZE];      // Circular buffer
  int start;                      // Start index
  int end;                       // End index (where the next value is inserted)
  int count;                      // Number of elements currently in buffer
} dataBuffer;

void update_buffer(void* reading, dataBuffer* buffer);

int buffer_median(dataBuffer* buffer, int start, int end);

#pragma endregion Events-Claculations

