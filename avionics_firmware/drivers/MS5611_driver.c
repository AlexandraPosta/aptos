/*
	Leeds University Rocketry Organisation - LURA
    Author Name: Evan Madurai
    Created on: 15 December 2023
    Description: Driver file for the Barometer module MS561101BA03 (https://www.te.com/usa-en/product-MS560702BA03-50.html)
*/

#include "MS5611_driver.h"
#include "stdint.h"
#include "mcu.h"

// min OSR by default
// static uint8_t pressAddr  = PRESSURE_OSR_256;
// static uint8_t tempAddr   = TEMP_OSR_256;
// static uint32_t convDelay = CONVERSION_OSR_256;
typedef struct PROM_data
{
    uint16_t blank;         // "factory data and the setup" ???
    uint16_t SENS;          // C1 - Pressure Sensitivity
    uint16_t OFF;           // C2 - Pressure Offset
    uint16_t TCS;           // C3 - Temperature coefficient of pressure sensitivity
    uint16_t TCO;           // C4 - Temperature coefficient of pressure offset
    uint16_t T_REF;         // C5 - Reference temperature
    uint16_t TEMPSENS;      // C6 - Temperature coefficient of the temperature 
    uint16_t SC_CRC         // Serial code and CRC
} PROM_data;

PROM_data ms5611_prom_data;

typedef struct M5611_data
{
    int32_t temp;
    int32_t pressure;
} M5611_data;

SPI_TypeDef* MS5611_SPI;

uint8_t MS5611_init(SPI_TypeDef* spi)
{
    MS5611_SPI = spi;
    spi_enable_cs(MS5611_SPI, MS5611_CS);
    uint8_t init = spi_transmit(MS5611_SPI, MS5611_CMD_RESET);
    printf("MS5611 init\r\n");
    spi_disable_cs(MS5611_SPI, MS5611_CS);
    delay_ms(2);
    MS5611_read_PROM(MS5611_SPI);
    M5611_data data;
    MS5611_get_data(&data);
	return 0;
}

int32_t MS5611_get_data_test()
{   
    //MS5611_read_PROM(MS5611_SPI);
    M5611_data data;
    MS5611_get_data(&data);
    printf("Temp: %u Pressure: %u \r\n", data.temp, data.pressure);
	return 0;
}


/* ---------------------------------- Private Functions ---------------------------------- */

uint16_t PROM_values[8];

uint8_t MS5611_read_PROM()
{
    // Take ptr to the PROM data struct
    int16_t *prom_ptr = (int16_t *)&ms5611_prom_data;
    for(int i = 0; i < 8; i++)
    {
        uint16_t result;
        spi_enable_cs(MS5611_SPI, MS5611_CS);
        spi_transmit_receive(MS5611_SPI, MS5611_CMD_READ_PROM(i), 1, 2, &result);
        spi_disable_cs(MS5611_SPI, MS5611_CS);
        // Fill struct using ptr arithmatic
        *(prom_ptr + i) = result;
    }
    
    return 0;
}

int MS5611_get_data(M5611_data* data)
{
    // check if the device has a register that checks if the conversion is complete?
    spi_enable_cs(MS5611_SPI, MS5611_CS);
    spi_transmit_receive(MS5611_SPI, MS5611_CMD_CONVERT_D2, 1, 0, NULL);
    spi_disable_cs(MS5611_SPI, MS5611_CS);

    delay_ms(8);
    uint32_t D2;
    spi_enable_cs(MS5611_SPI, MS5611_CS);
    spi_transmit_receive(MS5611_SPI, MS5611_CMD_READ_ADC, 1, 3, &D2);
    spi_disable_cs(MS5611_SPI, MS5611_CS);

    int32_t dT = (D2) - ((int32_t)ms5611_prom_data.T_REF << 8);
    int32_t TEMP = 2000 + dT * ms5611_prom_data.TEMPSENS / (2<<23);

    spi_enable_cs(MS5611_SPI, MS5611_CS);
    spi_transmit_receive(MS5611_SPI, MS5611_CMD_CONVERT_D1, 1, 0, NULL);
    spi_disable_cs(MS5611_SPI, MS5611_CS);

    delay_ms(8);

    uint32_t D1;
    spi_enable_cs(MS5611_SPI, MS5611_CS);
    spi_transmit_receive(MS5611_SPI, MS5611_CMD_READ_ADC, 1, 3, &D1);
    spi_disable_cs(MS5611_SPI, MS5611_CS);

    int64_t OFF = (ms5611_prom_data.OFF * pow(2,16)) + (ms5611_prom_data.TCO*dT)/pow(2,7);
    int64_t SENS = (ms5611_prom_data.SENS * pow(2,15)) + (ms5611_prom_data.TCS*dT)/pow(2,8);
    int32_t PRESSURE = (D1 * SENS / pow(2,21) - OFF) / pow(2,15);
    data->temp = TEMP;
    data->pressure = PRESSURE;

    return 0;
}
