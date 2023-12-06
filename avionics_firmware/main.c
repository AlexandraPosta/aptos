#include <stdio.h>
#include "mcu.h"
#include "NAND_flash_driver.h"
#include "STM32_init.h"


// Flags
FlightStages flightStage = LAUNCHPAD;

/**
  @brief Required for compilation
*/
static volatile uint32_t s_ticks;
void SysTick_Handler(void) {
  s_ticks++;
}


/**
  @brief TODO
*/
void update_sensors() {
  pass;
};


/**
  @brief TODO
*/
void send_data() {
  pass;
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
  // Initialise MCU
  return 0;
}
