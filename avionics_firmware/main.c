#include <stdio.h>
#include "mcu.h"
#include "STM32_init.h"
#include "stm32l4r5xx.h"


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
  @brief Initial Routine to run on hardware. Should trigger RGB blink sequence
  and Serial printing via LUART1  
*/
// TODO check pins
void run_test_routine() {  
  uint16_t led_B = PIN('H', 3);
  gpio_set_mode(led_B, GPIO_MODE_OUTPUT);
  pwr_vdd2_init();
  systick_init(FREQ / 1000);
  uart_init(LUART1, 9600);

  uint32_t timer = 0, period = 500;

  for (;;) {
    if (timer_expired(&timer, period, s_ticks)) {
      static bool on = true;                            // This block is executed
      gpio_write(led_B, on);                            // Every `period` milliseconds
      on = !on;                                         // Toggle LED state
      printf("LED: %d, tick: %lu\r\n", on, s_ticks);    // Write message
    }
  }
}


/**
  @brief Main entry point for the Flight Computer (HFC) firmware
*/
int main(void) {
  STM32_init();
  systick_init(FREQ / 1000);
  uart_init(LUART1, 9600);
  printf("==================== PROGRAM START ==================\r\n");
  run_test_routine();
  return 0;
}
