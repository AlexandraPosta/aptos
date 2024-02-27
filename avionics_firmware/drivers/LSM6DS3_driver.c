/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Tyler Green
    Created on: 01 February 2024
    Description: Driver file for the IMU module LSM6DS3 (https://www.mouser.co.uk/datasheet/2/389/dm00133076-1798402.pdf)
*/

#include "LSM6DS3_driver.h"


SPI_TypeDef *LSM6DS3_SPI;
LSM6DS3_DATA LSM6DS3_data;

#pragma region Public   //___________ Public _________________

int8_t LSM6DS3_init(SPI_TypeDef *spi){
    uint8_t LSM6DS3_retVal = 123;
    LSM6DS3_SPI = spi;
    // check if we recieve who am I correctly
    LSM6DS3_retVal = LSM6DS3_read_reg(LSM6DS3_WHO_AM_I_ADRS);
    if(LSM6DS3_retVal == LSM6DS3_WHO_AM_I_EXP){
        printf("LSM6DS3 knows who it is...");
        // power on the accelerometer
        LSM6DS3_write_reg(LSM6DS3_CTRL1_XL_ADRS, 0b01000000);
        LSM6DS3_write_reg(LSM6DS3_CTRL2_G_ADRS, 0b01000000);
    }
    else{
        printf(" LSM6DS3 thinks its %x ",LSM6DS3_retVal);
        printf("compared to: %x\r\n", LSM6DS3_WHO_AM_I_EXP);
        LSM6DS3_retVal = -1;
    }
    return LSM6DS3_retVal;
};

int8_t LSM6DS3_get_data(LSM6DS3_DATA *data){
    int8_t LSM6DS3_retVal = 1;
    spi_enable_cs(LSM6DS3_SPI, LSM6DS3_CS);
    spi_write_byte(LSM6DS3_SPI, LSM6DS3_TEMP_ADDRESS);
    data->LSM6DS3_TEMP = LSM6DS3_get_sens_val();
    data->LSM6DS3_GYRO_X = LSM6DS3_get_sens_val();
    data->LSM6DS3_GYRO_Y = LSM6DS3_get_sens_val();
    data->LSM6DS3_GYRO_Z = LSM6DS3_get_sens_val();
    data->LSM6DS3_ACC_X = LSM6DS3_get_sens_val();
    data->LSM6DS3_ACC_Y = LSM6DS3_get_sens_val();
    data->LSM6DS3_ACC_Z = LSM6DS3_get_sens_val();
    spi_disable_cs(LSM6DS3_SPI, LSM6DS3_CS);
    return LSM6DS3_retVal;
};

#pragma endregion Public

#pragma region Private

static int16_t LSM6DS3_get_sens_val(){
    int16_t val1, val2;
    val1 = (int16_t)spi_read_byte(LSM6DS3_SPI);
    val2 = (int16_t)spi_read_byte(LSM6DS3_SPI) << 8;

    //LSM6DS3_read_reg();
    return val1 && val2;
};

static uint8_t LSM6DS3_write_reg(uint8_t address, uint8_t data){
    //uint8_t response;
    //spi_transmit_receive(LSM6DS3_SPI, LSM6DS3_CS, address, 1, 0, &response);
    
    spi_enable_cs(LSM6DS3_SPI,LSM6DS3_CS);
    spi_write_byte(LSM6DS3_SPI,address);
    spi_write_byte(LSM6DS3_SPI,data);
    spi_disable_cs(LSM6DS3_SPI,LSM6DS3_CS);
    
    return 1;
};

static uint8_t LSM6DS3_read_reg(uint8_t address){
    printf("address to write to is: %d\r\n",address);
    address = address ^ 0x80;   // make sure first bit is 1
    printf("appending a 1, it becomes: %d\r\n",address);
    uint8_t LSM6DS3_retVal = 0;

    set_cs(LSM6DS3_CS);
    spi_transmit_receive(LSM6DS3_SPI, LSM6DS3_CS, address, 1, 1, &LSM6DS3_retVal);
    unset_cs(LSM6DS3_CS);
    /*
    spi_enable_cs(LSM6DS3_SPI, LSM6DS3_CS);
    spi_write_byte(LSM6DS3_SPI, address);

    LSM6DS3_retVal = spi_read_byte(LSM6DS3_SPI);
    spi_disable_cs(LSM6DS3_SPI,LSM6DS3_CS);
    */    
    return LSM6DS3_retVal;
};

#pragma endregion Private