/*
	Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta, Oliver Martin
  Created on: 27 Feb 2023
  Description: Main header file for the HFC firmware; suitable for STM32L4R5
*/
#ifndef MCU_H
#define MCU_H

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <math.h>

// https://github.com/STMicroelectronics/cmsis_device_l4/blob/master/Include/system_stm32l4xx.h
#include "stm32l4r5xx.h"
#include "STM32_init.h"


#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)
#define LOW 0
#define HIGH 1

typedef struct DateTime {
  uint8_t year;          // 0 - 128
  uint8_t month;         // 1 - 12
  uint8_t day;           // 1 - 32
  uint8_t hour;          // 0 - 23
  uint8_t minute;        // 0 - 59
  uint8_t second;        // 0 - 59
  uint16_t millisecond;  // 0 - 999
  uint16_t microsecond;  // 0 - 999
} DateTime;

typedef struct Vector3 {
  uint16_t x;
  uint16_t y;
  uint16_t z;
} Vector3;

typedef struct GNSS_Data{
  uint16_t latitude;
  uint16_t longitude;
  uint32_t heading1;
  uint32_t velocity;
} GNSS_Data;

// 128 bytes
typedef struct FrameArray {
  DateTime date;
  uint16_t changeFlag;  // IS THIS NEEDED? CAN THIS BE DONE BETTER?
  Vector3 accelHighG;
  Vector3 accelLowG;
  Vector3 gyroscope;
  uint32_t barometer;
  uint16_t thermocouple[4];
  uint16_t humidity;
  uint32_t temp;
  uint16_t magneticFieldStrength;
  GNSS_Data GNSS;
  uint16_t ADC[2];
  uint8_t hammingCode[8];
  uint16_t CRC_Check;
  int successFlag; // Not used in zip
} FrameArray;

extern volatile uint32_t s_ticks;

static void printf_float(char* name, float value) {
  char str[30];

  char *tmpSign = (value < 0) ? "-" : "";
  float tmpVal = (value < 0) ? -value : value;

  uint32_t tmpInt1 = (uint32_t) tmpVal;  // Get the integer (678).
  float tmpFrac = (tmpVal - tmpInt1);    // Get fraction (0.0123).
  int tmpInt2 = trunc(tmpFrac * 1000);   // Turn into integer (123).

  // Print as parts, note that you need 0-padding for fractional bit.
  sprintf(str, "%s = %s%d.%03d", name, tmpSign, tmpInt1, tmpInt2);
  printf("%s", str);
}

#pragma region System Clk
/**
  @brief The low level delay
*/
static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

static inline uint32_t get_time_us(){
  return TIM2->CNT;
}

static inline uint32_t get_time_ms(){
  return s_ticks;
}

/**
  @brief Delay in nanoseconds
  @param time Time in nanoseconds
*/
static inline void delay_nanoseconds(uint32_t time) {
  spin(time);
}

/**
  @brief Delay in microseconds
  @param time Time in microseconds
*/
static inline void delay_microseconds(uint32_t time) {
  uint32_t startTime = get_time_us();
  while ((get_time_us() - startTime) < time);
}

/**
  @brief Delay in miliseconds
  @param time Time in miliseconds
*/
static inline void delay_milliseconds(uint32_t time) {
  uint32_t initial_ticks = s_ticks; 
  while (s_ticks - initial_ticks < time); //hold until that many ticks have passed
}

static inline void delay(uint32_t time){
  uint32_t startTime = get_time_us();
  uint32_t delayPeriod = time*1000;
  while ((get_time_us() - startTime) < delayPeriod);
}



/**
  @brief Enable system clocks by setting frequency
  @param ticks Required frequency
*/
static inline void systick_init(uint32_t ticks) {
  if ((ticks - 1) > 0xffffff) return;         // Systick timer is 24 bit
  SysTick->LOAD = ticks - 1;
  SysTick->VAL = 0;
  SysTick->CTRL |= BIT(0) | BIT(1) | BIT(2);   // Enable systick, enable call back, set clk source to AHB
  s_ticks = 0;
  RCC->APB2ENR |= BIT(0);                     // Enable SYSCFG
}

#pragma endregion System Clk


#pragma region GPIO
#define GPIO(bank) ((GPIO_TypeDef *) (0x48000000 + 0x400 * (bank)))
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };


/**
  @brief Set the GPIO mode to input, output, alternate function or analog
  @param pin Selected pin
  @param mode Selected mode: GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG
*/
static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
  GPIO_TypeDef *gpio = GPIO(PINBANK(pin));  // GPIO bank
  int n = PINNO(pin);                       // Pin number
  RCC->AHB2ENR |= BIT(PINBANK(pin));        // Enable GPIO clock
  gpio->MODER &= ~(3U << (n * 2));          // Clear existing setting
  gpio->MODER |= (mode & 3U) << (n * 2);    // Set new mode
}


/**
  @brief Set the GPIO to an alternate function
  @param pin Selected pin
  @param af_num Selected alternative mode
*/
static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
  GPIO_TypeDef *gpio = GPIO(PINBANK(pin));  // GPIO bank
  int n = PINNO(pin);                       // Pin number
  gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
  gpio->AFR[n >> 3] |= ((uint32_t) af_num) << ((n & 7) * 4);
}


/**
  @brief Write to the GPIO
  @param pin Selected pin
  @param val Value to be written: True/False
*/
static inline void gpio_write(uint16_t pin, bool val) {
  GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
  gpio->BSRR |= (1U << PINNO(pin)) << (val ? 0 : 16);
}


/**
  @brief Read from the GPIO
  @param pin Selected pin
  @return Value of the GPIO
*/
static inline bool gpio_read(uint16_t pin) {
  GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
  bool value;
  value = gpio->IDR & (1U << PINNO(pin));
  return value;
}
#pragma endregion GPIO

#pragma region DFU
//test function found on thread here: https://stackoverflow.com/questions/66538837/stm32-usb-programming-jump-to-bootloader-for-dfu
//supposed to put MCU into DFU mode without having to pull boot0 pin high.

static inline void jump_to_bootloader(){
  //set LED 2 to blue
  gpio_write(RGB2_R, LOW);
  gpio_write(RGB2_G, LOW);
  gpio_write(RGB2_B, HIGH);

  SysTick->CTRL = SysTick->LOAD = SysTick->VAL = 0;
  SYSCFG->MEMRMP &= ~(SYSCFG_MEMRMP_MEM_MODE);
  SYSCFG->MEMRMP |= SYSCFG_MEMRMP_MEM_MODE_0;

  void (*SysMemBootJump)(void);
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) 0x1FFF0004));
	SysMemBootJump();
}

#pragma endregion DFU


#pragma region UART
#define UART1 USART1
#define UART2 USART2
#define UART3 USART3
#define LUART1 LPUART1


/**
  @brief Initialise the UART
  @param uart Selected UART (1, 2, 3 or low power)
  @param baud Baud rate
*/
static inline void uart_init(USART_TypeDef *uart, unsigned long baud) {
  uint8_t af = 8;           // Alternate function
  uint16_t rx, tx;  // pins

  if (uart == USART1) RCC->APB2ENR  |= RCC_APB2ENR_USART1EN; // BIT(14);   //Enables clock for UART 1
  if (uart == USART2) RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;// BIT(17);   //TODO find what needs to be done here

  // UART
  if (uart == USART1){   //SERVO USART1
    af = 7;
    tx = USART1_tx;
    rx = USART1_rx; 
  }  
  if (uart == USART2){   //GNSS USART2
    af = 7; 
    tx = USART2_tx;
    rx = USART2_rx; 
  }  

  gpio_set_mode(tx, GPIO_MODE_AF);
  gpio_set_af(tx, af);
  gpio_set_mode(rx, GPIO_MODE_AF);
  gpio_set_af(rx, af);
  uart->CR1 = 0;                                // Disable this UART                              
  uart->BRR = FREQ / baud;//256*FREQ / baud;    // FREQ is a CPU frequency
  uart->CR1 |= BIT(0) | BIT(2) | BIT(3);        // Set UE, RE, TE Datasheet 50.8.1 
}

/**
  @brief Initialise the UART in single-wire/half-duplex mode
  @param uart Selected UART
  @param baud Baud rate
*/
static inline void UartSingleWireInit(USART_TypeDef *uart, unsigned long baud) {
  uint8_t af = 8;           // Alternate function
  uint16_t rx;
  uint16_t tx;  // pins

  if (uart == USART1) RCC->APB2ENR  |= RCC_APB2ENR_USART1EN; // BIT(14);   //Enables clock for UART 1
  if (uart == USART2) RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;// BIT(17);   //TODO find what needs to be done here

  // UART
  if (uart == USART1){   //SERVO USART1
    af = 7;
    tx = USART1_tx;
    rx = USART1_rx; 
  }  
  if (uart == USART2){   //GNSS USART2
    af = 7; 
    tx = USART2_tx;
    rx = USART2_rx; 
  }  

  gpio_set_mode(tx, GPIO_MODE_AF);
  gpio_set_af(tx, af);
  gpio_set_mode(rx, GPIO_MODE_AF);
  gpio_set_af(rx, af);
  uart->CR1 = 0;                                      // Disable this UART                              
  uart->BRR = FREQ / baud;//256*FREQ / baud;          // FREQ is a CPU frequency
  uart->CR2 &= ~(USART_CR2_LINEN & USART_CR2_CLKEN);  // Clear LINEN & CLKEN bits in CR2 as required in datasheet
  uart->CR3 &= ~(USART_CR3_SCEN & USART_CR3_IREN);    // Clear SCEN & IREN bits in CR2 as required in datasheet
  uart->CR3 |= USART_CR3_HDSEL;                       // Set single wire/half duplex mode
  uart->CR1 |= BIT(0) | BIT(2) | BIT(3);              // Set UE, RE, TE Datasheet 50.8.1 
}

/**
  @brief Write via UART
  @param uart Selected UART (1, 2, 3 or low power)
  @param byte Byte to be written
*/
static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
  uart->TDR = byte;
  while ((uart->ISR & BIT(7)) == 0) spin(1);    // Ref manual STM32L4 50.8.10 USART status register (USART_ISR) 
}


/**
  @brief Write to UART buffer
  @param uart Selected UART (1, 2, 3 or low power)
  @param buf Buffer 
  @param len Length of the buffer
*/
static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
  while (len-- > 0) uart_write_byte(uart, *(uint8_t *) buf++);
}


/**
  @brief Set UART to read
  @param uart Selected UART (1, 2, 3 or low power)
  @return True when ready
*/
static inline int uart_read_ready(USART_TypeDef *uart) {
  return uart->ISR & BIT(5);  // If RXNE bit is set, data is ready Ref manual 50.8.10
}


/**
  @brief Read UART
  @param uart Selected UART (1, 2, 3 or low power)
  @return Byte from UART
*/
static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
  return (uint8_t) (uart->RDR & 255);
}
#pragma endregion UART


#pragma region CS

// Set all CS pins to high
static inline void unset_cs()
{
  // all CS pins HIGH
  gpio_write(CS0, HIGH);
  gpio_write(CS1, HIGH);
  gpio_write(CS2, HIGH);
  gpio_write(CS3, HIGH);
  gpio_write(CS4, HIGH);
}

//pull the one CS line low
static inline void set_cs(uint16_t pin_cs)
{
  unset_cs();
  gpio_write(pin_cs, LOW);
}

#pragma endregion CS


#pragma region SPI
/**
  @brief Initialisation of the SPI
  @param spi
*/
static inline void spi_init(SPI_TypeDef *spi) {
  //  STM32L4R5 Reference manual SPI Documentation:
  //  - RM0351,  pg 78-82: Memory map and peripheral register boundary
  //  - DS10198, pg 68: Pinout
  //  - RM0351,  pg 1459: Configuration of SPI
  //  - RM0351,  pg 1484: SPI register map
  //  - RM0351,  pg 1476: SPI registers
  //  STM32L4R5 alternative functions map: https://www.st.com/resource/en/datasheet/stm32l4r5vi.pdf

  uint8_t af;
  uint16_t sclk, miso, mosi;
  
  if (spi == SPI1){
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN, af = 5, sclk = PIN('A', 5), miso = PIN('A', 6), mosi = PIN('A', 7);
  }else if (spi == SPI2){
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN, af = 5, sclk = PIN('B', 13), miso = PIN('B', 14), mosi = PIN('B', 15);
  }
  gpio_set_mode(sclk, GPIO_MODE_AF);
  gpio_set_mode(miso, GPIO_MODE_AF);
  gpio_set_mode(mosi, GPIO_MODE_AF);

  gpio_set_af(sclk, af);
  gpio_set_af(miso, af);
  gpio_set_af(mosi, af);

  // MCU clock speed (FREQ) is 16 MHz and max MCU SPI speed is FREQ / 2.
  spi->CR1 &= ~(7U << 3); //Clears BR (bits 5:3) to 000 which is = system clock/2
  spi->CR1 |= (3U << 3); //Sets BR to 011, systemclk/16, so 1MHz

  // CPOL (clk polarity) and CPHA (clk phase) defaults  to produce the desired clock/data relationship
  // CPOL (clock polarity) bit controls the idle state value of the clock when no data is being transferred.
  //spi->CR1 &= ~BIT(0);
  //spi->CR1 &= ~BIT(1);
  spi->CR1 |= BIT(0);
  spi->CR1 |= BIT(1);

  // MCU datasheet "Select simplex or half-duplex mode by configuring
  // RXONLY or BIDIMODE and BIDIOE (RXONLY and BIDIMODE cannot be set
  // at the same time)"
  spi->CR1 &= ~BIT(10); //full duplex
  spi->CR1 &= ~BIT(15); //2line unidirectional data mode

  // Datasheet: "The MSB of a byte is transmitted first"
  spi->CR1 &= ~BIT(7);

  // CRC not needed so ignoring CRCL and CRCEN

  // Software slave management seems required
  spi->CR1 |= BIT(9); //manually do ss

  // Configuring the mcu as SPI master
  spi->CR1 |= BIT(2);

  // Frame size is 8 bits
  spi->CR2 |= (7U << 8);
  // spi->CR2 |= (15u << 8);

  // Activating SS output enable
  spi->CR2 |= BIT(2);
  // spi->CR2 |= BIT(3);

  //generate RXNE event when FIFO level is >= 8bits
  spi->CR2 |= BIT(12);

  // Not using TI protocol so not bothered by FRF bit
  // Not using NSSP protocol so not bothered by NSS bit
  // Not bothered by FRXTH bit. We're not reading anything on the mcu end
  // Not bothered about configuring the CRC polynomial
  // Not bothered about any DMA stuff

  // Enable the SPI!
  spi->CR1 |= BIT(6);
}

/**
  @brief Get the SPI ready for reading
  @param spi Selected SPI (1, 2 or 3)
  @return True when ready
*/
static inline int spi_ready_read(SPI_TypeDef *spi) {
  while (!(spi->SR & BIT(1))); // Wait until transmit buffer is empty
  while (!(spi->SR & BIT(0))); // Wait until receive buffer is not empty (RxNE, 52.4.9)
  return 1; // data is ready
}

static inline int spi_ready_write(SPI_TypeDef *spi) {
  while ((spi->SR & BIT(7))); // Wait until SPI is not busy
  return 1; // data is ready
}

/**
  @brief Enable chip select line for spi
  @param 
  @note Not needed but keeps compatibility with drivers
*/
static inline void spi_enable_cs(uint16_t cs) {
  set_cs(cs); 
}

/**
  @brief Enable chip select line for spi
  @param 
  @note Not needed but keeps compatibility with drivers
*/
static inline void spi_disable_cs(uint16_t cs)
{
  unset_cs();
}

/**
  @brief Transmit single byte to and recieve a byte from SPI peripheral
  @param spi Selected SPI (1, 2 or 3)
  @param send_byte Byte to be sent via SPI
  @return Byte from SPI
*/
static inline uint8_t spi_transmit(SPI_TypeDef *spi, uint8_t send_byte)
{
  uint8_t recieve_byte = 123;
  //printf("you want to send: %d", send_byte);
  spi_ready_write(spi);
  //*((volatile uint8_t *)&(spi->DR)) = send_byte << 8;
  *(volatile uint8_t *)&spi->DR = send_byte;
  //printf("Sent: %x\r\n", send_byte);
  // since SPI is asyncronous communication, we gotta recieve a bit aswell....
  spi_ready_read(spi);
  recieve_byte = *((volatile uint8_t *)&(spi->DR)); 
  //printf("Received: %x\r\n", recieve_byte);
  return recieve_byte;
}

static inline uint8_t spi_transmit_receive(SPI_TypeDef *spi, uint8_t *send_byte, uint8_t transmit_size, uint8_t receive_size, void* result_ptr)
{
  uint8_t ret_value = 0;
  spi_ready_write(spi);

  for (int i = 0; i<transmit_size; i++) {
    spi_transmit(spi, send_byte[i]);
  }

  uint32_t result = 0;
  int8_t rs = receive_size;
  while (rs > 0)
  {
    uint8_t received = spi_transmit(spi, 0x00);
    result = (result << 8);
    result = result | received;
    rs--;
    spi_ready_write(spi);
  }
  
  if(receive_size == 1)
  {
    *((uint8_t*)result_ptr) = result;
  } else if (receive_size == 2)
  {
    *((uint16_t*)result_ptr) = result;
  } else if (receive_size > 2)
  {
    *((uint32_t*)result_ptr) = result;
  }

  return ret_value;
}

/**
  @brief Transmit single byte to and from SPI peripheral
  @param spi Selected SPI (1, 2 or 3)
  @return Byte from SPI
*/
static inline uint8_t spi_read_byte(SPI_TypeDef *spi)
{
  uint8_t recieve_byte = 99;
  recieve_byte = spi_transmit(spi, 0x00);
  return recieve_byte;
}

/**
  @brief Transmit multiple bytes to and from SPI peripheral
  @param spi Selected SPI (1, 2 or 3)
  @param send_byte Byte to be sent via SPI
  @param transmit_size Number of bytes to be sent (Not currently implemented)
  @return error checking
*/
static inline uint8_t spi_write_buf(SPI_TypeDef *spi, uint8_t *send_bytes, uint8_t transmit_size)
{
  for(int i = 0; i < transmit_size; i++) {
    spi_transmit(spi, send_bytes[i]);
  }
  return 0;
}


static inline uint8_t spi_read_buf(SPI_TypeDef *spi, uint8_t *recieve_bytes, uint8_t receive_size){
  uint8_t retval = 0;
  uint8_t i = 0;
  while (i < receive_size)
  {
    *(recieve_bytes + i) = spi_read_byte(spi); // dereference to get element
    printf("Received: %x\r\n", *(recieve_bytes + i));
    i++;
    // printf("Received Value: %u  %u  %u \r\n", received, receive_size, result);
  }
  return retval; // TODO error checking
}


#pragma endregion SPI


/**
  @brief Set timer
  @param t Expiration time
  @param prd Period
  @param now Current time
  @return True when timer is done
*/
static inline bool timer_expired(uint32_t *t, uint32_t prd, uint32_t now) {
  if (now + prd < *t) *t = 0;                    // Time wrapped? Reset timer
  if (*t == 0) *t = now + prd;                   // First poll? Set expiration
  if (*t > now) return false;                    // Not expired yet, return
  *t = (now - *t) > prd ? now + prd : *t + prd;  // Next expiration time
  return true;                                   // Expired, return true
}


/**
  @brief Initialise the secondary power control register (Vdd2) which is needed for the GPIO G
*/
static inline void pwr_vdd2_init() {
  RCC->APB1ENR1 |= BIT(28);         // page 291
  PWR->CR2 |= BIT(9);               // set the IOSV bit in the PWR_CR2 page 186, 219
}

#pragma region watchdog
//information about watchdogs cann be found here:
//https://www.st.com/resource/en/product_training/STM32WB-WDG_TIMERS-Independent-Watchdog-IWDG.pdf 

/**
  @brief Starts the watchdog timer
*/
static inline void watchdog_init(){
  //Set the IWDG_SW option bit, This is to hardware enable the watchdog instead of enabling it each time like below
  //pretty sure the option register is write protected, there are steps to unlock. In reference manual: 3.4.2 pg141
  //FLASH->OPTR &= ~FLASH_OPTR_IWDG_SW;       //turn the hardware IWDG on by setting bit to off
  //FLASH->OPTR |= FLASH_OPTR_IWDG_STDBY;     //run IWDG (1 turns off stand by)
  //FLASH->OPTR |= FLASH_OPTR_IWDG_STOP;     //run IWDG (1 turns off stop)

  /*The first step is to write the Key register with value 0x0000 CCCC which starts the watchdog.
    Then remove the independent watchdog register protection by writing 0x0000 5555 to unlock the key.
    Set the independent watchdog prescaler in the IWDG_PR register by selecting the prescaler divider feeding the counter clock.
    Write the reload register (IWDG_RLR) to define the value to be loaded in the watchdog counter.
  */
  IWDG->KR = 0xCCCC;
  IWDG->KR = 0x5555;
  while(IWDG->SR & ~IWDG_SR_PVU_Msk){}; //prescalar can only be set when PVU bit is reset, so hold until = 0
  //IWDG->PR = 0x0001;  //Prescalar is 3 bits, 000 = /4, 001 = /8, 010 = /16, 011 = /32... Divides the 32kHz clock signal
  IWDG->PR = 0x0002;
  /*
  To calculate the counter reload value to achieve the desired reset time limit the following formula is used:
  RL = (Desired_Time_ms * 32,000)/(4 * 2^PR * 1000) -1
  RL has a limit of 4095, so choose a PR to get a value less than this
  So for a 0.5s time:
  RL = (500 * 32,000)/(4 * 2^(1) * 1000) - 1 = 1999
  */
  while(IWDG->SR & ~IWDG_SR_RVU_Msk){};  //reload value can only be set when RVU bit is reset, so hold until = 0
  IWDG->RLR = 0x7CF;  //1999, value to be reloaded into the counter on reset
}

/**
  @brief Reset the watchdog timer to prevent a system reset
*/
static inline void watchdog_pat(){
  //IWDG_KR register must be written with 0x0000AAAA
  IWDG->KR = 0xAAAA;

}

#pragma endregion watchdog

#endif