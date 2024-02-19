/*
	Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta, Oliver Martin
  Created on: 11 June 2023
  Description: STM32L4R5 class
*/

#include "STM32_init.h"
#include "mcu.h"

/**
  @brief Initialisation of the STM32L5 board
*/
void STM32_init()
{
  //set system clock speed
  //MSI range can only be set if MSI is off, or MSI is on and MSIRDY = 1
  RCC->CR |= RCC_CR_MSION;       //set to 1 for MSI on
  while ((RCC->CR & RCC_CR_MSION) && !(RCC->CR & RCC_CR_MSIRDY)); //wait until off, or on and ready
  RCC->CR = (RCC->CR & ~RCC_CR_MSIRANGE_Msk)  | RCC_CR_MSIRANGE_11; //set MSI range to 48Hz (0b1011)
  RCC->CR |= RCC_CR_MSIRGSEL;    //set to 1 to use MSI range from CR register

  systick_init(FREQ / 1000);     // Tick every 1 ms
  STM32_init_peripherals();
  STM32_init_internals();
}


/**
  @brief Initialisation of the STM32L4R5 board internals (UART, SPI, Power, etc.)
*/
void STM32_init_internals()
{
  // UART
  systick_init(FREQ / 1000);  // Tick every 1 ms
  //uart_init(LUART1, 115200);  // Initialise Low Power UART;
  //uart_init(UART1,  115200);  // Initialise UART1;
  //uart_init(UART2,  115200);  // Initialise UART2;
  //uart_init(UART3,  115200);  // Initialise UART3;

  // SPI TODO
  spi_init(SPI1);
  spi_init(SPI2);

  // Additional
  pwr_vdd2_init();            // Initialise VDD2 for GPIO G
}


/**
  @brief Initialisation of the STM32L5 board externals (GPIO, ADC, etc.)
*/
void STM32_init_peripherals()
{
  // Define inputs and outputs
  gpio_set_mode(_buzzer, GPIO_MODE_OUTPUT);
  gpio_set_mode(_blueLED, GPIO_MODE_OUTPUT);
  gpio_set_mode(_boot0, GPIO_MODE_OUTPUT);

  gpio_set_mode(RGB1_R, GPIO_MODE_OUTPUT);
  gpio_set_mode(RGB1_G, GPIO_MODE_OUTPUT);
  gpio_set_mode(RGB1_B, GPIO_MODE_OUTPUT);
  gpio_set_mode(RGB2_R, GPIO_MODE_OUTPUT);
  gpio_set_mode(RGB2_G, GPIO_MODE_OUTPUT);
  gpio_set_mode(RGB2_B, GPIO_MODE_OUTPUT);

  // Initialise
  gpio_write(_blueLED, HIGH);
  gpio_write(_buzzer, LOW);

}


/**
  @brief Led on
*/
void STM32_led_on()
{
  gpio_write(_blueLED, HIGH);
}


/**
  @brief Led off
*/
void STM32_led_off()
{
  gpio_write(_blueLED, LOW);
}


/**
  @brief Buzzer sound
*/
void STM32_beep_buzzer(uint32_t onDurationMs, uint32_t offDurationMs, uint16_t noOfBeeps)
{
  for (int i = 0; i < noOfBeeps; i++) {
      gpio_write(_buzzer, HIGH);
      delay_microseconds(onDurationMs);
      gpio_write(_buzzer, LOW); 
      delay_microseconds(offDurationMs);
  }
}


/**
  @brief Buzzer sound to indicate power on
*/
void STM32_indicate_on_buzzer()
{
  STM32_beep_buzzer(100, 50, 3);
}


/**
  @brief Led light to indicate power on
*/
void STM32_indicate_on_led()
{
  STM32_led_on();
  delay_microseconds(200);
  STM32_led_off();
  delay_microseconds(200);
  STM32_led_on();
  delay_microseconds(200);
  STM32_led_off();
}

