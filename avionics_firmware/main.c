#include <stdio.h>
#include "STM32_init.h"
#include "mcu.h"
#include "stm32l4r5xx.h"


// Flags
FlightStages flightStage = LAUNCHPAD;

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
  @brief Initial Routine to run on hardware. Should trigger RGB blink sequence
  and Serial printing via LUART1  
*/
void run_test_routine2() {
  char buf[] = "test";
  uart_write_buf(UART1, buf, 4);
  watchdog_pat();
}

void run_test_routine() {
  watchdog_pat();
  static bool on = true;
  int counter = 0;
  while (1){
    delay(500);  
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

void run_test_routine3() {
  watchdog_pat();
  while (1){
    if (s_ticks % 1000 > 500){
        STM32_led_on(); 
    }else{
      STM32_led_off();
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
  
  //watchdog_init();
  watchdog_pat();
  STM32_led_off();
  //delay_microseconds(1);
  //STM32_indicate_on_buzzer();
  //STM32_indicate_on_led();
  

  watchdog_pat();
  gpio_write(RGB1_G, HIGH);
  gpio_write(RGB2_R, HIGH);

  run_test_routine();

  return 0;
}
