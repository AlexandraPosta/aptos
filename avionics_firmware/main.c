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
  @brief Initial Routine to run on hardware. Should trigger RGB blink sequence
  and Serial printing via LUART1  
*/
// TODO check pins
void run_test_routine() {  
  
  //why is this included so many times???? systick_init(FREQ / 1000);

  uint32_t timer = 0, period = 500;

  for (;;) {
    if (timer_expired(&timer, period, s_ticks)) {       // This block is executed every `period` milliseconds
      static bool on = true;                            
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
  @brief Main entry point for the Flight Computer (HFC) firmware
*/
int main(void) {
  STM32_init();
  //why is this here as well??? systick_init(FREQ / 1000);
  //This is included in stm32_init() if needed uart_init(LUART1, 9600);
  //printf("==================== PROGRAM START ==================\r\n");
  //cs_init();
  
  STM32_indicate_on_buzzer();
  STM32_indicate_on_led();
  STM32_led_on();

  gpio_write(RGB1_G, HIGH);
  gpio_write(RGB2_R, HIGH);
  //delay_microseconds(10000);
  //run_test_routine();
  return 0;
}
