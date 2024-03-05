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
  //run_test_routine_LSM6DS3();
  //run_test_routine_MS5611();
  //run_nand_flash_test();
  
  

  return 0;
}
