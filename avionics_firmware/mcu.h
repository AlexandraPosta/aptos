/*
	Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 27 Feb 2023
  Description: Main header file for the HFC firmware; suitable for STM32L4R5
*/

#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// https://github.com/STMicroelectronics/cmsis_device_l4/blob/master/Include/system_stm32l4xx.h
#include "stm32l4r5xx.h"

#define FREQ 4000000  
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

#define LOW 0
#define HIGH 1


#pragma region System Clk
/**
  @brief The low level delay
*/
static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
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
  delay_nanoseconds(time*1000);
}


/**
  @brief Delay in miliseconds
  @param time Time in miliseconds
*/
static inline void delay(uint32_t time) {
  delay_microseconds(time*1000);
}


/**
  @brief Enable system clocks by setting frequency
  @param ticks Required frequency
*/
static inline void systick_init(uint32_t ticks) {
  if ((ticks - 1) > 0xffffff) return;         // Systick timer is 24 bit
  SysTick->LOAD = ticks - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = BIT(0) | BIT(1) | BIT(2);   // Enable systick
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
  uint16_t rx = 0, tx = 0;  // pins

  if (uart == UART1) RCC->APB2ENR  |= BIT(14);   
  if (uart == UART2) RCC->APB1ENR1 |= BIT(17);   
  if (uart == UART3) RCC->APB1ENR1 |= BIT(18);   
  if (uart == LUART1) {
    RCC->APB1ENR2 |= BIT(0); 
  }

  if (uart == UART1) af = 7, tx = PIN('A', 9), rx = PIN('A', 10);
  if (uart == UART2) af = 7, tx = PIN('A', 2), rx = PIN('A', 3);
  if (uart == UART3) af = 7, tx = PIN('D', 8), rx = PIN('D', 9); 
  if (uart == LUART1) af = 8, tx = PIN('G', 7), rx = PIN('G', 8);   

  gpio_set_mode(tx, GPIO_MODE_AF);
  gpio_set_af(tx, af);
  gpio_set_mode(rx, GPIO_MODE_AF);
  gpio_set_af(rx, af);
  uart->CR1 = 0;                                // Disable this UART                              
  uart->BRR = 256*FREQ / baud;                  // FREQ is a CPU frequency
  uart->CR1 |= BIT(0) | BIT(2) | BIT(3);        // Set UE, RE, TE Datasheet 50.8.1 
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


#pragma region SPI
/**
  @brief Initialisation of the SPI
  @param spi
*/
static inline void spi_init(SPI_TypeDef *spi) {
  // STM32L4R5 Reference manual SPI Documentation (from page ):
  //  - RM0351, pg 78-82: Memory map and peripheral register boundary
  //  - DS10198, pg 68: Pinout
  //  - RM0351, pg 1459: Configuration of SPI
  //  - RM0351, pg 1484: SPI register map
  //  - RM0351, pg 1476: SPI registers
  //  - NUCLEO Pinout: https://os.mbed.com/platforms/ST-Nucleo-L476RG/#nucleo-pinout)

  uint8_t af;
  uint16_t ss, sclk, miso, mosi;

  if (spi == SPI1) RCC->APB2ENR  |= BIT(12), af = 5, ss = PIN('A', 4), sclk = PIN('A', 5),  miso = PIN('A', 6),  mosi = PIN('A', 7);
  if (spi == SPI2) RCC->APB1ENR1 |= BIT(14), af = 5, ss = PIN('B', 12), sclk = PIN('B', 13), miso = PIN('B', 14), mosi = PIN('B', 15);
  if (spi == SPI3) RCC->APB1ENR1 |= BIT(15), af = 6, ss = PIN('A', 15), sclk = PIN('C', 10), miso = PIN('C', 11), mosi = PIN('C', 12);

  gpio_set_mode(ss, GPIO_MODE_AF);
  gpio_set_mode(sclk, GPIO_MODE_AF);
  gpio_set_mode(miso, GPIO_MODE_AF);
  gpio_set_mode(mosi, GPIO_MODE_AF);

  gpio_set_af(ss, af);
  gpio_set_af(sclk, af);
  gpio_set_af(miso, af);
  gpio_set_af(mosi, af);

  // MCU clock speed (FREQ) is 4 MHz and max MCU SPI speed is FREQ / 2.
  spi->CR1 &= ~(7U << 3);

  // CPOL (clk polarity) and CPHA (clk phase) defaults  to produce the desired clock/data relationship
  // CPOL (clock polarity) bit controls the idle state value of the clock when no data is being transferred. 
  spi->CR1 &= ~BIT(0);
  spi->CR1 &= ~BIT(1);

  // MCU datasheet "Select simplex or half-duplex mode by configuring
  // RXONLY or BIDIMODE and BIDIOE (RXONLY and BIDIMODE cannot be set
  // at the same time)"
  spi->CR1 &= ~BIT(10);
  spi->CR1 &= ~BIT(15);

  // Datasheet: "The MSB of a byte is transmitted first" 
  spi->CR1 &= ~BIT(7);

  // CRC not needed so ignoring CRCL and CRCEN

  // Software slave management seems required
  spi->CR1 |= BIT(9);

  // Configuring the mcu as SPI master
  spi->CR1 |= BIT(2);

  // Frame size is 8 bits
  spi->CR2 |= (7U << 8);

  // Activating SS output enable
  spi->CR2 |= BIT(2);

  // Not using TI protocol so not bothered by FRF bit
  // Not using NSSP protocol so not bothered by NSS bit
  // Not bothered by FRXTH bit. We're not reading anything on the mcu end
  // Not bothered about configuring the CRC polynomial
  // Not bothered about any DMA stuff

  // Enable the SPI!
  spi->CR1 |= BIT(6);
}


/**
  @brief Write via SPI
  @param spi `SPI1`, `SPI2` or `SPI3`
  @param byte Byte to write
*/
static inline void spi_write_byte(SPI_TypeDef *spi, uint8_t byte) {
  spi->DR = byte;
  while ((spi->SR & BIT(7)) != 0) spin(1);
}


/**
  @brief Write to SPI buffer
  @param spi Selected SPI (1, 2 or 3)
  @param buf Buffer to write to
  @param len Length of the message
*/
static inline void spi_write_buf(SPI_TypeDef *spi, char *buf, size_t len) {
  while (len-- > 0) spi_write_byte(spi, *(uint8_t *) buf++);
}


/**
  @brief Get the SPI ready for reading
  @param spi Selected SPI (1, 2 or 3)
  @return True when ready
*/
static inline int spi_ready_read(SPI_TypeDef *spi) {
  while (!(spi->SR & BIT(1)));    // Wait until transmit buffer is empty
  while (!(spi->SR & BIT(0)));    // Wait until receive buffer is not empty (RxNE, 52.4.9)

  return 1;                       // data is ready
}


/**
  @brief Read from the selected SPI
  @param spi Selected SPI (1, 2 or 3)
  @return Byte from SPI
*/
static inline uint16_t spi_read_byte(SPI_TypeDef *spi) {
  return (uint16_t) (spi->DR & 255);
}


/**
  @brief Test that the SPI works appropriately; use Putty to check the LUART output
  @param spi Selected SPI (1, 2 or 3)
*/
/*
static inline uint16_t spi_test_routine(SPI_TypeDef *spi, uint16_t valueToSend) {
  spi = SPI1;
  valueToSend++;

  // Convert the integer to a byte array
  uint8_t byteBuffer[sizeof(valueToSend)];
  for (size_t i = 0; i < sizeof(valueToSend); ++i) {
    byteBuffer[i] = (uint8_t)(valueToSend >> (i * 8)) & 0xFF;
  }

  // Calculate the length of the byte array
  size_t bufferLength = sizeof(byteBuffer);
  
  spi_write_buf(SPI1, (char *)byteBuffer, bufferLength);

  // Wait for transfer to complete (until receive buffer is not empty)
  spi_ready_read(SPI1); 

  // Read received data from SPI
  uint16_t receivedValue = spi_read_byte(SPI1);

  // Convert the received byte array back to an integer
  for (size_t i = 0; i < sizeof(receivedValue); ++i) {
    receivedValue |= ((uint16_t)byteBuffer[i] << (i * 8));
  }

  // Print the received integer
  printf("Received Value: %hu\r\n", receivedValue);

  return 0;
}*/

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