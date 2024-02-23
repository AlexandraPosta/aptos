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


PROM_data ms5611_prom_data;

SPI_TypeDef* MS5611_SPI;

uint8_t MS5611_init(SPI_TypeDef* spi)
{
    MS5611_SPI = spi;
    set_cs(MS5611_CS);
    uint8_t init = spi_write_byte(MS5611_SPI, MS5611_CMD_RESET);
    printf("MS5611 init\r\n");
    unset_cs();
    delay_microseconds(10);
    MS5611_read_PROM(MS5611_SPI);
    M5611_data data;
    MS5611_get_data(&data);
    printf("Temp: %u, \tPressure: %u\r\n", data.temp, data.pressure);
	return 0;
}

int32_t MS5611_get_data_test()
{   
    //MS5611_read_PROM(MS5611_SPI);
    M5611_data data;
    MS5611_get_data(&data);
    printf("Temp: %u, \tPressure: %u\r\n", data.temp, data.pressure);
	return 0;
}


/* ---------------------------------- Private Functions ---------------------------------- */

uint16_t PROM_values[8];

uint8_t MS5611_read_PROM()
{
    uint16_t *prom_ptr = (uint16_t *)&ms5611_prom_data;

    for(int i = 0; i < 8; i++)
    {   
        set_cs(MS5611_CS);
        spi_write_byte(MS5611_SPI,  MS5611_CMD_READ_PROM(i));
        volatile uint8_t rx_buf[2];
        spi_read_buf(MS5611_SPI, &rx_buf, 2);
        unset_cs();
        uint16_t result = (rx_buf[0] << 8) | rx_buf[1];
        //uint16_t result = spi_transmit_receive(MS5611_SPI, MS5611_CS, MS5611_CMD_READ_PROM(i), 1, 2);
        *(prom_ptr + i) = result;
    }
    
    return 0;
}

int32_t MS5611_get_data(M5611_data* data)
{
    int32_t PRESSURE;
    int32_t TEMP;

    // check if the device has a register that checks if the conversion is complete?
    set_cs(MS5611_CS);
    spi_write_byte(MS5611_SPI, MS5611_CMD_CONVERT_D2);
    unset_cs(); 

    delay_ms(9);
    spi_clear_read_buf(MS5611_SPI);

    set_cs(MS5611_CS);
    spi_write_byte(MS5611_SPI, MS5611_CMD_READ_ADC);
    //delay_microseconds(10);

    uint8_t recieve[3];
    spi_read_buf(MS5611_SPI, &recieve, 3);
    int32_t D2 = (recieve[0] << 16) | (recieve[1] << 8) | (recieve[2]);
    unset_cs();
    int32_t dT = (D2) - ((int32_t)ms5611_prom_data.T_REF << 8);
    int32_t BASE_TEMP = (( dT * ms5611_prom_data.TEMPSENS) >> 23);
    TEMP = 2000 + BASE_TEMP;
    printf("TEMP:%u\r\n", TEMP);


    //PRESSURE:

    // does not work
    //spi_transmit_receive(MS5611_SPI, MS5611_CS, MS5611_CMD_CONVERT_D1, 1, 1);
    set_cs(MS5611_CS);
    spi_write_byte(MS5611_SPI, MS5611_CMD_CONVERT_D1);
    //spi_read_byte(MS5611_SPI);
    unset_cs();
    //delay_microseconds(100);

    set_cs(MS5611_CS);
    spi_write_byte(MS5611_SPI, MS5611_CMD_READ_ADC);
    spi_read_buf(MS5611_SPI, &recieve, 3);
    int32_t D1 = (recieve[0] << 16) | (recieve[1] << 8) | (recieve[2]);
    unset_cs();

    int64_t OFF = ((((int64_t)ms5611_prom_data.OFF << 16) + ((int64_t)ms5611_prom_data.TCO*dT))>>7);
    int64_t SENS = ((((int64_t)ms5611_prom_data.SENS<<15) + ((int64_t)ms5611_prom_data.TCS*dT))>>3);

    int32_t T2 = 0, OFF2 = 0, SENS2 = 0;
    int32_t BASE_TEMP_SQUARED = BASE_TEMP*BASE_TEMP;

    // Second order compensation
    if(TEMP < 2000)
    {
        // Low Temperature
        T2 = ((int64_t)dT * (int64_t)dT) >> 31;
        OFF2 = 5 * BASE_TEMP_SQUARED >> 1;
        SENS2 = 5 * BASE_TEMP_SQUARED >> 2;
        if(TEMP<-1500)
        {
            // Very low temperature
            OFF2 = OFF2 + 7 * ((TEMP+1500)*(TEMP+1500));
            SENS2 = (SENS2 + 11 * ((TEMP+1500)*(TEMP+1500))) >> 1;
        }
    }

    TEMP=TEMP-T2;
    OFF=OFF-OFF2;
    SENS=SENS-SENS2;

    //PRESSURE = ((((int64_t)D1 * SENS) >> 21) - OFF) >> 16;
    PRESSURE = ((((int64_t)D1 * SENS) >> 21) - OFF) >> 15;

    //printf("D1: %ld, OFF: %ld, SENS: %ld, PRESSURE: %ld \r\n", D1, (uint32_t)OFF, (uint32_t)SENS, PRESSURE);

    data->temp = TEMP;
    data->pressure = PRESSURE;

    return 0;   
}
