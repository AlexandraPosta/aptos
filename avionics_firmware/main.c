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


  
  delay_ms(200);
  delay_ms(1000);
  //run_test_routine_BME280();
  //ADXL375_init(SPI2);
  //run_ADXL375_routine();
  run_test_routine_LSM6DS3();
  //run_test_routine_MS5611();
  //run_nand_flash_erase();
  //NAND_flash_test_routine();
  

  return 0;
}
