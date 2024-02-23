#include <stdio.h>
#include "STM32_init.h"
#include "mcu.h"
#include "stm32l4r5xx.h"
#include "drivers/MS5611_driver.h"

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
      delay_ms(onDurationMs);
      gpio_write(_buzzer, LOW); 
      delay_ms(offDurationMs);
  }
}

/**
  @brief Buzzer sound to indicate power on
*/
void STM32_indicate_on_buzzer()
{
  STM32_beep_buzzer(100, 100, 3);
}

/**
  @brief Led light to indicate power on
*/
void STM32_indicate_on_led()
{
  STM32_led_on();
  delay_ms(200);
  STM32_led_off();
  delay_ms(100);
  STM32_led_on();
  delay_ms(200);
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
void run_test_routine3() {
  
  MS5611_init(SPI2);
  while (1){
    MS5611_get_data_test();
    watchdog_pat();
    delay_ms(500);
  }
}


/**
  @brief Main entry point for the Flight Computer (HFC) firmware
*/
int main(void) {
  STM32_init();
  uart_init(USART1, 115200);

  spi_init(SPI2);

  printf("==================== PROGRAM START ==================\r\n");
  
  watchdog_init();
  watchdog_pat();

  STM32_led_off();

  STM32_indicate_on_buzzer();
  watchdog_pat();
  STM32_indicate_on_led();

  watchdog_pat();
  gpio_write(RGB1_G, HIGH);
  gpio_write(RGB2_R, HIGH);

  run_test_routine3();

  return 0;
}
