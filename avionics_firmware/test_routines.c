/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta, Oliver Martin
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

void run_test_routine_MS5611() {
  
  MS5611_init(SPI2);
  M5611_data data;
  while (1){
    MS5611_get_data(&data);
    printf("Pressure: %i\r\n", data.pressure);
    watchdog_pat();
    delay_milliseconds(10);
  }
}

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

  Lsm6ds3Init(SPI2, &gyro_data);
  orientation_init(&_orientation, &gyro_data);
  
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

      orientation_update(dt, &_orientation, &gyro_data, 1);
      printf_float("E, p:", _orientation.current_euler.pitch/M_PI_F*180, true);
      printf_float(", r: ", _orientation.current_euler.roll/M_PI_F*180, true);
      printf_float(", y: ", _orientation.current_euler.yaw/M_PI_F*180, true);
      printf("\r\n");
    }
    watchdog_pat();
    
  }
}

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
  delay_milliseconds(100);
  ServoUartInit(UART1);
  SmartServo servo1 = ServoInit(UART1, 1);
  SmartServo servo2 = ServoInit(UART1, 102);
  SmartServo servo3 = ServoInit(UART1, 103);
  SmartServo servo4 = ServoInit(UART1, 104);
  ServoSetPID(&servo1, 32, 32, 0);
  ServoSetAccel(&servo1, 250);
  delay_milliseconds(300);
  int count = 0;
  int angle = 0;
  ServoSetTargetAngle(&servo1, 15000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo1, 0);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo2, 15000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo2, 0);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo3, 15000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo3, 0);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo4, 15000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo4,0);
  while(1){}
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
  ServoDeflections _servoDeflection;
  _servoDeflection.servo_deflection_1 = 0;
  _servoDeflection.servo_deflection_2 = 0;
  _servoDeflection.servo_deflection_3 = 0;
  _servoDeflection.servo_deflection_4 = 0;

  SmartServo servo1 = ServoInit(UART1, 1);
  SmartServo servo2 = ServoInit(UART1, 102);
  SmartServo servo3 = ServoInit(UART1, 103);
  SmartServo servo4 = ServoInit(UART1, 104);

  ServoSetTargetAngle(&servo1, 15*1000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo1, 0);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo2, 15*1000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo2, 0);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo3, 15*1000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo3, 0);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo4, 15*1000);
  delay_milliseconds(500);
  ServoSetTargetAngle(&servo4,0);

  // Initialise IMU 
  Lsm6ds3Init(SPI2, &_LSM6DS3_data);

  for (;;) {
    //delay_microseconds(250000); // 0.25 second delay
    newTimer = get_time_us();

    // Read the gyro data
    Lsm6ds3GyroRead(SPI2, &_LSM6DS3_data);
    Lsm6ds3AccRead(SPI2, &_LSM6DS3_data);

    // Return the Euler and Quaternion angles in microseconds
    orientation_update((newTimer - oldTimer), &_orientation, &_LSM6DS3_data, 0);

    // Perform LQR control
    LQR_update_gain(&_LQR_controller, 35);
    LQR_perform_control(&_LQR_controller, _orientation, &_servoDeflection);

    ServoSetTargetAngle(&servo1, (int32_t)_servoDeflection.servo_deflection_3*10);
    ServoSetTargetAngle(&servo2, (int32_t)_servoDeflection.servo_deflection_4*10);
    ServoSetTargetAngle(&servo3, (int32_t)_servoDeflection.servo_deflection_2*10);
    ServoSetTargetAngle(&servo4, (int32_t)_servoDeflection.servo_deflection_1*10);

    oldTimer = newTimer;
  }
}