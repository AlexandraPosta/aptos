#include <stdio.h>
#include "STM32_init.h"
#include "mcu.h"
#include "stm32l4r5xx.h"
#include "drivers/MS5611_driver.h"
//#include "drivers/BME280_driver.h"
#include "drivers/LSM6DS3_driver.h"
#include "drivers/ADXL375_driver.h"
#include "drivers/NAND_flash_driver.h"
#include "test_routines.h"
#include "data_buffer.h"

// Flags
FlightStages flightStage = LAUNCHPAD;
volatile uint32_t s_ticks;
extern void SysTick_Handler(void) {
  s_ticks++;
}

#pragma region Updates
void get_frame_array(FrameArray* _frameArray, 
                    M5611_data* _M5611_data, 
                    ADXL375_data* _ADXL375_data) {
  // Convert data to frame TODO
  Vector3 _acc_high_g = { _ADXL375_data->x, _ADXL375_data->y, _ADXL375_data->z };

  // Add time stamp
  _frameArray->date.minute = (get_time_us()/(1000000*60))%60; //minuts
  _frameArray->date.second = (get_time_us()/1000000)%60; //seconds
  _frameArray->date.millisecond = (get_time_us()/1000)%1000; //milli seconds
  _frameArray->date.microsecond = get_time_us()%1000; //Mirco seconds
  // Add data to the frame
  _frameArray->barometer = _M5611_data->pressure;
  _frameArray->temp = _M5611_data->temp;
  _frameArray->accelHighG = _acc_high_g;  
}

void update_sensors(M5611_data* _M5611_data, 
                    ADXL375_data* _ADXL375_data) {
  MS5611_get_data(_M5611_data);
  ADXL375_get_data(_ADXL375_data);
}
#pragma endregion Updates

void run_nand_flash_erase(){
  init_flash();
  watchdog_pat();
  read_flash_ID();
  watchdog_pat();

  erase_all();
}

/**
  @brief Routine to test NAND Flash reading and writing.
*/
void NAND_flash_test_routine()
{
  printf("==================== START WRITING ====================\r\n");
  init_flash();
  
  uint8_t dataArray[128];
  _memset(dataArray, 0x0, 128);
  /*
  for (uint8_t i = 0; i < 128; i ++) {
    dataArray[i] = i;
  }
  */
  FrameArray _input = unzip(dataArray);

  int numOfFramesToTest = 5;
  for (int i = 0; i < numOfFramesToTest; i++) {

    dataArray[0] = 0;
    dataArray[1] = 0;
    _input.date.minute = (get_time_us()/(1000000*60))%60; //minuts
    _input.date.second = (get_time_us()/1000000)%60; //seconds
    _input.date.millisecond = (get_time_us()/1000)%1000; //milli seconds
    _input.date.microsecond = get_time_us()%1000; //Mirco seconds
    _input.accelHighG.x = i;
    _input.accelHighG.y = i*50;
    _input.accelHighG.z = 100 + i*20;
    
    log_frame(_input);
    printf("======================== DONE ========================\r\n");
  }
  printf("==================== DONE WRITING ====================\r\n");
  
  read_all_csv();
  print_capacity_info();
}

/**
  @brief Main entry point for the Flight Computer (HFC) firmware
*/
int main(void) {
  // STM32 setup
  STM32_init();
  uart_init(USART1, 921600);
  spi_init(SPI2);

  printf("==================== PROGRAM START ==================\r\n");
  //watchdog_init();
  watchdog_pat();
  STM32_led_off();
  STM32_indicate_on_buzzer();
  watchdog_pat();
  STM32_led_on();
  gpio_write(RGB1_G, HIGH);
  gpio_write(RGB2_R, HIGH);

  printf("============ INITIALISE NAND FLASH ============\r\n");
  init_flash();

  printf("============== INITIALISE DRIVERS =============\r\n");
  // Sensor initialisation
  MS5611_init(SPI1);          // Barometer
  ADXL375_init(SPI1);         // Accelerometer
  
  // Buffer
  FrameArray frame;                         // initialise the frameArray that keeps updating
  uint8_t dataArray[128];                   // dummy array to store the frame data
  _memset(dataArray, 0, sizeof(dataArray)); // set the necessary memory and set values to 0
  zip(frame, dataArray);                    // convert from normal array into FrameArray
  dataBuffer frame_buffer;                  // contains FrameArrays
  init_buffer(&frame_buffer);               // initialise the buffer

  // Buffer data
  M5611_data _M5611_data;
  ADXL375_data _ADXL375_data;

  // Additional variables
  int _data[WINDOW_SIZE];
  int previous_value = 999999999;
  int current_value = 999999999;
  int apogee_incr = 3;

  //printf("============== ADD TESTS HERE ==============\r\n");
  //delay_ms(200);
  //delay_ms(1000);
  //run_test_routine_BME280();
  //ADXL375_init(SPI2);
  //run_ADXL375_routine();
  //run_test_routine_LSM6DS3();
  //run_test_routine_MS5611();
  //run_nand_flash_erase();
  //NAND_flash_test_routine();
  
  printf("============= ENTER MAIN PROCEDURE ============\r\n");
  for (;;) {
    switch (flightStage) {
        case LAUNCHPAD:
          // Get the sensor readings
          update_sensors(&_M5611_data, &_ADXL375_data);
          get_frame_array(&frame, &_M5611_data, &_ADXL375_data); 

          // Update buffer and window
          update_buffer(&frame, &frame_buffer);
          if (frame_buffer.count > WINDOW_SIZE*2) {
            // Get the window barometer median
            for (int i = 0; i < WINDOW_SIZE; i++) {
              _data[i] = frame_buffer.window[i].barometer;
            }
            current_value = get_median(_data, WINDOW_SIZE); // get pressure median

            // Check for launch given pressure decrease
            if ((frame_buffer.ground_ref - current_value) > LAUNCH_THRESHOLD) {
              flightStage = ASCENT;

              // Log all data from the buffer
              for (int i = 0; i < BUFFER_SIZE; i++) {
                log_frame(frame_buffer.frames[i]);
              }
            }
          }
          break;
        
        case ASCENT:
          // TODO: set the sampling rate

          // Get the sensor readings
          update_sensors(&_M5611_data, &_ADXL375_data);
          get_frame_array(&frame, &_M5611_data, &_ADXL375_data);

          // Log data
          log_frame(frame);

          // Update buffer and window  
          update_buffer(&frame, &frame_buffer);

          // Get window median readings
          for (int i = 0; i < WINDOW_SIZE; i++) {
            _data[i] = frame_buffer.window[i].barometer;
          }
          current_value = get_median(_data, WINDOW_SIZE); // get pressure median

          // Check for apogee given pressure increase
          if (current_value - previous_value > 0)
            flightStage = APOGEE;
          else
            previous_value = current_value;
          break;

        case APOGEE:
          // Get the sensor readings
          update_sensors(&_M5611_data, &_ADXL375_data);
          get_frame_array(&frame, &_M5611_data, &_ADXL375_data); 

          // Log data
          log_frame(frame);

          // Update buffer and window  
          update_buffer(&frame, &frame_buffer);

          // Run for a few cycles to record apogee when switch to descent
          if (apogee_incr == 0)
            flightStage = DESCENT;
          else
            apogee_incr--;
          break;

        case DESCENT:
          // TODO: reduce the sampling rate

          // Get the sensor readings
          update_sensors(&_M5611_data, &_ADXL375_data);
          get_frame_array(&frame, &_M5611_data, &_ADXL375_data); 

          // Log data
          log_frame(frame);

          // Update buffer and window  
          update_buffer(&frame, &frame_buffer);
          
          // Get window median readings
          int _data[WINDOW_SIZE];
          for (int i = 0; i < WINDOW_SIZE; i++) {
            _data[i] = frame_buffer.window[i].barometer;
          }

          // Check for landing
          if (is_stationary(_data)) {
            flightStage = LANDING;
          }
          break;

      case LANDING:
        // Beep continuously
        break;
    }
  }

  return 0;
}
