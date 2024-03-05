#include "data_buffer.h"
#include "drivers/ADXL375_driver.h"
#include "drivers/MS5611_driver.h"

// FIFO Buffer logic for data extraction
void update_buffer(void* reading, dataBuffer* buffer) {
  buffer->readings[buffer->end] = reading;
  buffer->end = (buffer->end + 1) % BUFFER_SIZE;
  if (buffer->count < BUFFER_SIZE) {
    buffer->count++;
  } else {
    buffer->start = (buffer->start + 1) % BUFFER_SIZE;
  }
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int buffer_median(dataBuffer* buffer, int start, int end)
{
    int size = end-start;
    qsort(buffer->readings, size, sizeof(int), cmpfunc);

    for(int i = 0; i < size; i++)
    {
        printf("%d\r\n", buffer->readings[i]);
    }
}