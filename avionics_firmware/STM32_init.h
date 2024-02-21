/*
	Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta, Oliver Martin
  Created on: 10 June 2023
  Description: Header file for the STM32L4R5 firmware
*/
#ifndef STM32_INIT_H
#define STM32_INIT_H

extern int FREQ;

// ------------ Pin Assignments -------------
#define _buzzer     PIN('B', 1)   //assigned for aptos board
#define _blueLED    PIN('E', 7)   //assigned for aptos board
#define _boot0      PIN('H', 3)   //Boot0 pin
#define _servoEn    PIN('B', 2);  //Servo enable pin
#define _USB_P      PIN('A', 12); //USB D+ line
#define _USB_N      PIN('A', 11); //USB D- line

//RGB LEDs
#define RGB1_R      PIN('E', 9)
#define RGB1_G      PIN('E', 10)
#define RGB1_B      PIN('E', 8)

#define RGB2_R      PIN('E', 12)
#define RGB2_G      PIN('E', 13)
#define RGB2_B      PIN('E', 11)

//SPI pins
#define SPI1_sclk   PIN('A', 5)
#define SPI1_miso   PIN('A', 6)
#define SPI1_mosi   PIN('A', 7)

#define SPI2_sclk   PIN('B', 13)
#define SPI2_miso   PIN('B', 14)
#define SPI2_mosi   PIN('B', 15)

//USART Pins
#define USART1_tx   PIN('B', 6);
#define USART1_rx   PIN('B', 7);
#define USART2_tx   PIN('D', 5);
#define USART2_rx   PIN('D', 6);

// Map cs pins to sensors
#define CS0         PIN('C', 7)   // Accelerometer
#define CS1         PIN('C', 9)   // IMU
#define CS2         PIN('A', 9)   // Barometer
#define CS3         PIN('A', 10)  // Temperature 
#define CS4         PIN('B', 12)  // SD card

//Interrupt pins
#define INT_GPS     PIN('D', 3)
#define INT_Accel   PIN('C', 8)
#define INT_IMU     PIN('A', 8)

//GPS Pins
#define nGPS_Reset  PIN('D', 4)
#define GPS_CLK     PIN('H', 0)

//Telemetry GPIO
#define Telemetry_Reset PIN('E', 5)
#define Telemetry_GPIO1 PIN('E', 4)
#define Telemetry_GPIO2 PIN('E', 3)

//NAND flash pins
#define NAND_D0     PIN('D', 15)
#define NAND_D1     PIN('D', 14)
#define NAND_D2     PIN('D', 13)
#define NAND_D3     PIN('D', 12)
#define NAND_D4     PIN('D', 11)
#define NAND_D5     PIN('D', 10)
#define NAND_D6     PIN('D', 9)
#define NAND_D7     PIN('D', 8)
#define NAND_ALE    PIN('D', 0)
#define NAND_WE#    PIN('D', 1)
#define NAND_WP#    PIN('D', 2)
#define NAND_RB#    PIN('C', 10)
#define NAND_RE#    PIN('C', 11)
#define NAND_CLE    PIN('C', 12)

#include "mcu.h"

// System parameters
#define SEA_LEVEL_PRESSURE_AT_SITE 1013.25 // change on the day
#define MSL_ALT_TIMER_THRESHOLD_MS 60000 // 1 minute

// Required structures
typedef enum FlightStages {LAUNCHPAD, ASCEND, APOGEE, DESCENT, LANDING} FlightStages;


typedef struct Date 
{
  unsigned int year;
  unsigned int month;
  unsigned int day;
} Date;


typedef struct Time
{
  unsigned int hour;
  unsigned int minute;
  unsigned int second;
} Time;


// Functions
/**
  @brief TODO
*/
void STM32_init(void);

/**
  @brief Sets the system clock frequency 
  @param frequency - Pass either RCC_CFGR_SW_MSI, RCC_CFGR_SW_HSI, or RCC_CFGR_SW_PLL
*/
void STM32_init_clock(unsigned long frequency);


/**
  @brief TODO
*/
void STM32_init_internals(void);


/**
  @brief TODO
*/
void STM32_init_peripherals(void);


/**
  @brief TODO
  @param onDurationMs
  @param offDurationMs
  @param noOfBeeps
*/
void STM32_beep_buzzer(uint32_t onDurationMs, uint32_t offDurationMs, uint16_t noOfBeeps);


/**
  @brief TODO
*/
void STM32_indicate_on_buzzer();


/**
  @brief TODO
*/
void STM32_indicate_on_led();


/**
  @brief TODO
*/
void STM32_led_on();


/**
  @brief TODO
*/
void STM32_led_off();


#endif /* STM32_init_H */
