/*
	Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 10 June 2023
  Description: Header file for the STM32L4R5 firmware
*/
#ifndef STM32_DRIVER_H
#define STM32_INIT_H
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
void STM32_indicate_on_red_led();


/**
  @brief TODO
*/
void STM32_leds_on();


/**
  @brief TODO
*/
void STM32_leds_off();


/**
  @brief TODO
*/
double STM32_get_battery_capacity(uint8_t batteryNo);


#endif /* STM32_DRIVER_H */
