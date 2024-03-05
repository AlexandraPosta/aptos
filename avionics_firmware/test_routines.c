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

void run_nand_flash_test(){
  init_flash();
  set_control_pins(WRITE_PROTECT_OFF);
  watchdog_pat();
  read_flash_status();
  watchdog_pat();
  read_flash_ID();
  watchdog_pat();
  
  FrameArray testFrame;
  (&testFrame)->temp = 0xAA;
  (&testFrame)->barometer = 10000;
  log_frame(testFrame);
  write_protection();
  printf("Write complete");

  watchdog_pat();
  delay_ms(300);
  watchdog_pat();
  //print_frame_array(recall_frame(0));

  read_all();
  watchdog_pat();
}


/**
  @brief Routine to test NAND Flash reading and writing.
*/
void NAND_flash_test_routine()
{
  printf("==================== START WRITING ====================\r\n");
  set_control_pins(WRITE_PROTECT);      // Write Protection
  set_control_pins(WRITE_PROTECT_OFF);  // Write Protection Off

  uint8_t dataArray[128];
  _memset(dataArray, 0x0, 128);

  for (uint8_t i = 0; i < 128; i ++) {
    dataArray[i] = i;
  }

  erase_block(0);   // erase block 0, use for debugging
  // erase_all();   // erase all blocks, but takes 2 minutes

  //write_frame(0, dataArray);        // testing  
  //read_frame(10000, dataArray, 8);  // testing
  FrameArray _input = unzip(dataArray);
  FrameArray _output;

  int data_intact = 0;
  int data_fixed = 0;
  int data_error = 0;
  int startAddr = frameAddressPointer;

  int numOfFramesToTest = 100;
  for (int i = 0; i < numOfFramesToTest; i++) {
    for (uint8_t j = 0; j < 128; j ++) {
      dataArray[j] = j;
    }

    dataArray[0] = 0;
    dataArray[1] = 0;

    _input = unzip(dataArray);
    log_frame(_input);
    printf("======================== DONE ========================\r\n");
  }
  printf("==================== DONE WRITING ====================\r\n");
  
  read_all();
  print_capacity_info();
}
