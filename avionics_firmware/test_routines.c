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
    delay_milliseconds(50);  
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
    delay_milliseconds(500);
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
    delay_milliseconds(500);
  }
}

void run_test_routine_LSM6DS3()
{
  printf("----------- IMU TEST -----------\r\n");
  LSM6DS3_data gyro_data;
  orientation_data _orientation;
  //delay_milliseconds(50);
  Lsm6ds3Init(SPI2, &gyro_data);
  orientation_init(&_orientation, &gyro_data);
  printf("A, X: %i, Y: %i, Z:%i \r\n", gyro_data.x_accel, gyro_data.y_accel, gyro_data.z_accel);

  uint32_t start_time = get_time_us();
  uint32_t current_time;
  uint32_t dt;
  while (1){
    current_time = get_time_us();
    dt = current_time - start_time;
    if(dt > 1000000/100){
      start_time = current_time;
      Lsm6ds3GyroRead(SPI2, &gyro_data);
      Lsm6ds3AccRead(SPI2, &gyro_data);
      //printf("A, X: %i, Y: %i, Z:%i \r\n", gyro_data.x_accel, gyro_data.y_accel, gyro_data.z_accel);
      orientation_update(dt, &_orientation, &gyro_data);
      /*
      printf("R, X: %i, Y: %i, Z:%i \r\n", gyro_data.x_rate, gyro_data.y_rate, gyro_data.z_rate);
      printf_float("Q, W:", _orientation.current_quaternion.w);
      printf_float(", X: ", _orientation.current_quaternion.x);
      printf_float(", Y: ", _orientation.current_quaternion.y);
      printf_float(", Z: ", _orientation.current_quaternion.z);
      printf("\r\n");*/
      printf_float("E, p:", _orientation.current_euler.pitch/M_PI_F*180);
      printf_float(", r: ", _orientation.current_euler.roll/M_PI_F*180);
      printf_float(", y: ", _orientation.current_euler.yaw/M_PI_F*180);
      printf("\r\n");
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
    delay_milliseconds(100);
    
    ADXL375_get_data(&_data);
    printf("X:%i, Y:%i, Z:%i\r\n", _data.x, _data.y, _data.z);
    
  }
}

void timer_test(){
  while(1){
    printf("%d\r\n",get_time_us());
    delay_milliseconds(50);
  }
}

void ServoTest(){
  //ServoEnable(true);
  delay_milliseconds(100);
  ServoUartInit(UART1);
  SmartServo servo1 = ServoInit(UART1, 104);
  SmartServo servo2 = ServoInit(UART1, 103);
  ServoSetPID(&servo1, 32, 32, 0);
  ServoSetAccel(&servo1, 250);
  delay_milliseconds(300);
  int count = 0;
  int angle = 0;
  while (1){
    angle = ((count%5)*90000)-180000;
    printf("Angle: %i\r\n", angle);
    ServoSetTargetAngle(&servo1, angle);
    ServoSetTargetAngle(&servo2, angle);
    count ++;
    delay_milliseconds(1000);
  }
  ServoSetTargetAngle(&servo1,90000);
  ServoSetTargetAngle(&servo2,-90000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo1,-10000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo2,10000);
  while(1){

  }
}

void DFU_programming_check(){
  printf("============== DFU flash check ===========\r\n");
  delay_milliseconds(500);
  while (uart_read_ready(USART1)){
    uint8_t recieve = uart_read_byte(USART1);
    delay(1);
    //printf("%x\r\n", recieve);
    if (recieve == 'f'){
      printf("Flash command recieved!\r\n");
      delay_milliseconds(500);
      jump_to_bootloader();
    }
  }
  printf("No flash request\r\n");
}

void run_controller_routine(LSM6DS3_data _LSM6DS3_data, orientation_data _orientation, LQR_controller _LQR_controller) {
  // Initialise timer
  uint32_t newTimer = get_time_us();
  uint32_t oldTimer = get_time_us();

  // Initialise servos
  char servoDeflection[4] = {0};
  SmartServo servo1 = ServoInit(UART1, 101);
  SmartServo servo2 = ServoInit(UART1, 102);
  SmartServo servo3 = ServoInit(UART1, 103);
  SmartServo servo4 = ServoInit(UART1, 104);
  delay_milliseconds(300);
  ServoSetTargetAngle(&servo3, 30*1000);
  delay_milliseconds(200);
  ServoSetTargetAngle(&servo3, 0*1000);
  delay_milliseconds(200);
  ServoSetTargetAngle(&servo4, 30*1000);
  delay_milliseconds(200);
  ServoSetTargetAngle(&servo4, 0*1000);

  // Initialise IMU 
  Lsm6ds3Init(SPI2, &_LSM6DS3_data);

  for (;;) {
    //delay_microseconds(250000); // 0.25 second delay
    newTimer = get_time_us();

    // Read the gyro data
    Lsm6ds3GyroReadAngle(SPI2, &_LSM6DS3_data);

    /*
    printf_float(" x", _LSM6DS3_data.x_rate);
    printf_float(" y", _LSM6DS3_data.y_rate);
    printf_float(" z", _LSM6DS3_data.z_rate);
    printf("\r\n");
    */

    // Return the Euler and Quaternion angles in microseconds
    orientation_update((newTimer - oldTimer), &_orientation, &_LSM6DS3_data);

    // Perform LQR control
    LQR_perform_control(&_LQR_controller, _orientation, servoDeflection);
    /*
    printf_float(" Servo 1", servoDeflection[0]);
    printf_float(" Servo 2", servoDeflection[1]);
    printf_float(" Servo 3", servoDeflection[2]);
    printf_float(" Servo 4", servoDeflection[3]);
    printf("\r\n");
    printf("\r\n");
    */

    servoDeflection[1] = _orientation.current_euler.pitch*10;
    servoDeflection[2] = _orientation.current_euler.roll*10;
    ServoSetTargetAngle(&servo1, (int32_t)servoDeflection[0]*1000);
    ServoSetTargetAngle(&servo2, (int32_t)servoDeflection[1]*1000);
    ServoSetTargetAngle(&servo3, (int32_t)servoDeflection[2]*1000);
    ServoSetTargetAngle(&servo4, (int32_t)servoDeflection[3]*1000);
    delay_milliseconds(10);

    // TODO
    // Set servos to 0 deflection if angle to vertical is low
    // Check if gain timer reached update interval
    // Calculate velocity based on barometer data
    // Update gains based on velocity

    oldTimer = newTimer;
  }
}