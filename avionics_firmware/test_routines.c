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
    delay_miliseconds(50);  
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
    delay_miliseconds(500);
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
    delay_miliseconds(500);
  }
}

void run_test_routine_LSM6DS3()
{
  printf("----------- IMU TEST -----------\r\n");
  LSM6DS3_data gyro_data;
  //delay_miliseconds(50);
  Lsm6ds3Init(SPI2, &gyro_data);
  uint32_t start_time = get_time_us();
  uint32_t current_time;
  while (1){
    current_time = get_time_us();
    if(current_time - start_time > 1000000/500){
      start_time = current_time;
      Lsm6ds3GyroReadAngle(SPI2, &gyro_data);
      //lsm6ds3AccRead(SPI2, &gyro_data);
      //lsm6ds3GyroRead(SPI2, &gyro_data);
    }
    watchdog_pat();
    
  }
}
/**
  @brief Routine to test the ADXL375 accelerometer.
*/
void run_ADXL375_routine()
{
  printf("================ ADXL375_routine ================\r\n");
  ADXL375_data _data;
  
  while(1){
    delay_miliseconds(100);
    
    ADXL375_get_data(&_data);
    printf("X:%i, Y:%i, Z:%i\r\n", _data.x, _data.y, _data.z);
    
  }
}

void timer_test(){
  while(1){
    printf("%d\r\n",get_time_us());
    delay_miliseconds(50);
  }
}

void ServoTest(){
  //ServoEnable(true);
  delay_miliseconds(100);
  ServoUartInit(UART1);
  SmartServo servo1 = ServoInit(UART1, 104);
  SmartServo servo2 = ServoInit(UART1, 103);
  ServoSetPID(&servo1, 32, 32, 0);
  ServoSetAccel(&servo1, 250);
  delay_miliseconds(300);
  int count = 0;
  int angle = 0;
  while (1){
    angle = ((count%5)*90000)-180000;
    printf("Angle: %i\r\n", angle);
    ServoSetTargetAngle(&servo1, angle);
    ServoSetTargetAngle(&servo2, angle);
    count ++;
    delay_miliseconds(1000);
  }
  ServoSetTargetAngle(&servo1,90000);
  ServoSetTargetAngle(&servo2,-90000);
  delay_miliseconds(500);
  ServoSetTargetAngle(&servo1,-10000);
  delay_miliseconds(500);
  ServoSetTargetAngle(&servo2,10000);
  while(1){

  }
}


void DFU_programming_check(){
  printf("============== DFU flash check ===========\r\n");
  delay_miliseconds(500);
  while (uart_read_ready(USART1)){
    uint8_t recieve = uart_read_byte(USART1);
    delay(1);
    //printf("%x\r\n", recieve);
    if (recieve == 'f'){
      printf("Flash command recieved!\r\n");
      delay_miliseconds(500);
      jump_to_bootloader();
    }
  }
  printf("No flash request\r\n");
}

