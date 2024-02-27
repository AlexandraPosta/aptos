#include <stdio.h>
#include "STM32_init.h"
#include "mcu.h"
#include "stm32l4r5xx.h"
#include "drivers/MS5611_driver.h"
#include "drivers/BME280_driver.h"
#include "drivers/LSM6DS3_driver.h"
#include "drivers/ADXL375_driver.h"

// Flags
FlightStages flightStage = LAUNCHPAD;
volatile uint32_t s_ticks;

/**
  @brief Required for compilation
*/

extern void SysTick_Handler(void) {
  s_ticks++;
}
/**
  @brief TODO
*/
void update_sensors() {
  return;
};


/**
  @brief TODO
*/
void send_data() {
  return;
};


/**
  @brief TODO
*/
void toggle_timeout_flag()
{

}

/**
  @brief Buzzer sound
  @param onDurationMs
  @param offDurationMs
  @param noOfBeeps
*/
void STM32_beep_buzzer(uint32_t onDurationMs, uint32_t offDurationMs, uint16_t noOfBeeps)
{
  for (int i = 0; i < noOfBeeps; i++) {
      gpio_write(_buzzer, HIGH);
      STM32_led_on();
      delay_ms(onDurationMs);
      gpio_write(_buzzer, LOW); 
      STM32_led_off();
      delay_ms(offDurationMs);
  }
}

/**
  @brief Buzzer sound to indicate power on
*/
void STM32_indicate_on_buzzer()
{
  STM32_beep_buzzer(50, 30, 3);
}

/**
  @brief Led light to indicate power on
*/
void STM32_indicate_on_led()
{
  STM32_led_on();
  delay_ms(100);
  STM32_led_off();
  delay_ms(100);
  STM32_led_on();
  delay_ms(100);
  STM32_led_off();
}


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
  int8_t ret_val = 123;
  ret_val = LSM6DS3_init(SPI2);
  printf("completed: %d \r\n ", ret_val);

  LSM6DS3_DATA imu_data;
  while(1){
    LSM6DS3_get_data(&imu_data);

    printf("Gyro X: %d \tGyro Y: %d, Gyro Z: %d\r\n ", (&imu_data)->LSM6DS3_GYRO_X, (&imu_data)->LSM6DS3_GYRO_Y, (&imu_data)->LSM6DS3_GYRO_Z);
    watchdog_pat();
    delay_ms(500);
  }
}

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



/**
  @brief Main entry point for the Flight Computer (HFC) firmware
*/
int main(void) {
  STM32_init();
  //uart_init(USART1, 115200);
  uart_init(USART1, 921600);

  spi_init(SPI2);

  printf("==================== PROGRAM START ==================\r\n");
  
  watchdog_init();
  watchdog_pat();

  STM32_led_off();

  STM32_indicate_on_buzzer();
  watchdog_pat();
  STM32_led_on();

  gpio_write(RGB1_G, HIGH);
  gpio_write(RGB2_R, HIGH);
  
  delay_ms(100);
  run_test_routine_BME280();
  //run_test_routine_ADXL375();
  //run_test_routine_LSM6DS3();
  //run_test_routine_MS5611();
  

  return 0;
}
