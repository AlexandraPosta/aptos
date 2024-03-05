#include <stdio.h>
#include "STM32_init.h"
#include "mcu.h"
#include "stm32l4r5xx.h"
#include "drivers/MS5611_driver.h"
#include "drivers/BME280_driver.h"
#include "drivers/LSM6DS3_driver.h"
//#include "drivers/ADXL375_driver.h"

#include "drivers/NAND_flash_driver.h"
#include "test_routines.h"

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
  @brief Main entry point for the Flight Computer (HFC) firmware
*/
int main(void) {
  STM32_init();
  //uart_init(USART1, 115200);
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
  
  delay_ms(100);
  //run_test_routine_BME280();
  //run_test_routine_ADXL375();
  run_test_routine_LSM6DS3();
  //run_test_routine_MS5611();
  //run_nand_flash_test();
  

  return 0;
}
