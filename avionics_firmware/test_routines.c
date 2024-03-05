/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 03 March 2024
  Description: Header Drivers test routines.
*/

#include "test_routines.h"


/**
  @brief Test Routine
*/
void run_test_routine2() {
  char buf[] = "test";
  printf("hello\r\n");
  uart_write_buf(USART1, buf, 4);
  watchdog_pat();
}

void run_test_routine() {
  watchdog_pat();
  static bool on = true;
  int counter = 0;
  while (1){
    delay_ms(50);  
    counter = counter + 1;
    if (counter < 20){
      watchdog_pat();
      if (on){
        STM32_led_on(); 
      }else{
        STM32_led_off();
      }               
      on = !on;     // Toggle LED state
    }
  }
}

/**
  @brief Test Routine
*/
void run_test_routine_MS5611() {
  
  MS5611_init(SPI2);
  while (1){
    MS5611_get_data_test();
    watchdog_pat();
    delay_ms(500);
  }
}
/**
  @brief Test Routine
*/
void run_test_routine_BME280() {
  printf("----STARTING BME280 TEST----\r\n");
  BME280_dev BME_dev;
  BME280_data BME_data;
  int8_t rslt = BME280_init(&BME_dev, SPI2);
  BME280_error_codes_print_result("bme280_interface_selection", rslt);
  while (1){
    rslt = BME280_get_data(BME280_TEMP, &BME_data, &BME_dev);
    BME280_error_codes_print_result("Get data:", rslt);

    printf("Return: %i\r\n", rslt);
    printf("TEMP: %u, \tPressure: %u, \tHumidity: %u\r\n", (&BME_data)->temperature, (&BME_data)->pressure, (&BME_data)->humidity);
    watchdog_pat();
    delay_ms(500);
  }
}

void run_test_routine_LSM6DS3()
{
  delay_ms(500);
  lsm6dsoDetect(SPI2);
  delay_ms(1000);
  lsm6dsoConfig(SPI2);
  watchdog_pat();
  delay_ms(1000);
  while (1){
    watchdog_pat();
    lsm6dsoGyroRead(SPI2);
    lsm6dsoAccRead(SPI2);
    delay_ms(200);
  }
}
/*
void run_test_routine_ADXL375()
{
  int8_t ret_val = 123;
  ret_val = ADXL375_init(SPI2);
  printf("INIT completed: %d \r\n ", ret_val);

  ADXL375_data accel_data;
  while(1){
    accel_data = ADXL375_get_data();

    printf("X: %d\tY: %d, Z: %d\r\n ", (&accel_data)->x, (&accel_data)->y, (&accel_data)->z);
    watchdog_pat();
    delay_ms(500);
  }
}

*/


