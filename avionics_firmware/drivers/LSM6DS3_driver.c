/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Tyler Green
    Created on: 01 February 2024
    Description: Driver file for the IMU module LSM6DS3 (https://www.mouser.co.uk/datasheet/2/389/dm00133076-1798402.pdf)
*/

#include "LSM6DS3_driver.h"

uint8_t lsm6dsoDetect(SPI_TypeDef *spi)
{
    uint8_t chipID = 0;
    uint8_t send_data[1] = {(LSM6DSO_REG_WHO_AM_I) | 0x80}; 
    spi_enable_cs(spi, LSM6DS3_CS);
    delay_ms(10);
    //spi_transmit_receive(spi, LSM6DSO_REG_WHO_AM_I, &chipID, 1, 1, &rslt);
    spi_transmit_receive(spi, &send_data, 1, 1, &chipID);
    spi_disable_cs(spi, LSM6DS3_CS);

    if (chipID == LSM6DS3_WHO_AM_I_EXP) {
        printf("LSM6DS3 Correct ChipID\r\n");
        return 1;
    }else{
        printf("LSM6DS3: Incorrect ChipID\r\n");
    }
    return 0;
}

static void lsm6dsoWriteRegister(SPI_TypeDef *spi, uint8_t registerID, uint8_t value, unsigned delayMs)
{
    uint8_t send_data[2] =  {registerID, value};
    spi_enable_cs(spi, LSM6DS3_CS);
    spi_transmit_receive(spi, &send_data, 2, 0, send_data);
    spi_disable_cs(spi, LSM6DS3_CS);
    if (delayMs) {
        delay_ms(delayMs);
    }
}

static void lsm6dsoWriteRegisterBits(SPI_TypeDef *spi, uint8_t registerID, uint8_t mask, uint8_t value, unsigned delayMs)
{
    uint8_t newValue;
    spi_enable_cs(spi, LSM6DS3_CS);
    spi_transmit_receive(spi, &registerID, 1, 1, &newValue); //get current data
    spi_disable_cs(spi, LSM6DS3_CS);
    
    delay_microseconds(delayMs);
    newValue = (newValue & ~mask) | value;
    lsm6dsoWriteRegister(spi, registerID, newValue, delayMs);
}

void lsm6dsoConfig(SPI_TypeDef *spi){
    // Reset the device (wait 100ms before continuing config)
    lsm6dsoWriteRegisterBits(spi, LSM6DSO_REG_CTRL3_C, LSM6DSO_MASK_CTRL3_C_RESET, BIT(0), 100);

    // Configure interrupt pin 1 for gyro data ready only
    //lsm6dsoWriteRegister(spi, LSM6DSO_REG_INT1_CTRL, LSM6DSO_VAL_INT1_CTRL, 1);

    // Disable interrupt pin 2
    lsm6dsoWriteRegister(spi, LSM6DSO_REG_INT2_CTRL, LSM6DSO_VAL_INT2_CTRL, 1);

    // Configure the accelerometer
    // 833hz ODR, 16G scale, use LPF1 output
    lsm6dsoWriteRegister(spi, LSM6DSO_REG_CTRL1_XL, (LSM6DSO_VAL_CTRL1_XL_ODR833 << 4) | (LSM6DSO_VAL_CTRL1_XL_16G << 2) | (LSM6DSO_VAL_CTRL1_XL_LPF1 << 1), 1);

    // Configure the gyro
    // 6664hz ODR, 2000dps scale
    lsm6dsoWriteRegister(spi, LSM6DSO_REG_CTRL2_G, (LSM6DSO_VAL_CTRL2_G_ODR6664 << 4) | (LSM6DSO_VAL_CTRL2_G_2000DPS << 2), 1);

    // Configure control register 3
    // latch LSB/MSB during reads; set interrupt pins active high; set interrupt pins push/pull; set 4-wire SPI; enable auto-increment burst reads
    lsm6dsoWriteRegisterBits(spi, LSM6DSO_REG_CTRL3_C, LSM6DSO_MASK_CTRL3_C, (LSM6DSO_VAL_CTRL3_C_BDU | LSM6DSO_VAL_CTRL3_C_H_LACTIVE | LSM6DSO_VAL_CTRL3_C_PP_OD | LSM6DSO_VAL_CTRL3_C_SIM | LSM6DSO_VAL_CTRL3_C_IF_INC), 1);

    // Configure control register 4
    // enable accelerometer high performane mode; set gyro LPF1 cutoff to 335.5hz
    lsm6dsoWriteRegisterBits(spi, LSM6DSO_REG_CTRL4_C, LSM6DSO_MASK_CTRL4_C, (LSM6DSO_VAL_CTRL4_C_I2C_DISABLE | LSM6DSO_VAL_CTRL4_C_LPF1_SEL_G), 1);

    // Configure control register 6
    // disable I2C interface; enable gyro LPF1
    lsm6dsoWriteRegisterBits(spi, LSM6DSO_REG_CTRL6_C, LSM6DSO_MASK_CTRL6_C, (LSM6DSO_VAL_CTRL6_C_XL_HM_MODE | LSM6DSO_VAL_CTRL6_C_FTYPE_335HZ), 1);

    // Configure control register 9
    // disable I3C interface
    lsm6dsoWriteRegisterBits(spi, LSM6DSO_REG_CTRL9_XL, LSM6DSO_MASK_CTRL9_XL, LSM6DSO_VAL_CTRL9_XL_I3C_DISABLE, 1);
    return;
}

/*
void lsm6dsoExtiHandler(extiCallbackRec_t *cb)
{
    gyroDev_t *gyro = container_of(cb, gyroDev_t, exti);
    gyro->dataReady = true;
}*/

bool lsm6dsoAccRead(SPI_TypeDef *spi)
{
    enum {
        IDX_ACCEL_XOUT_L,
        IDX_ACCEL_XOUT_H,
        IDX_ACCEL_YOUT_L,
        IDX_ACCEL_YOUT_H,
        IDX_ACCEL_ZOUT_L,
        IDX_ACCEL_ZOUT_H,
        BUFFER_SIZE, //6
    };

    uint8_t lsm6dso_rx_buf[BUFFER_SIZE];
    uint8_t send_data =  LSM6DSO_REG_OUTX_L_A;
    spi_enable_cs(spi, LSM6DS3_CS);
    spi_transmit_receive(spi, &send_data, 1, 1, &lsm6dso_rx_buf);
    for (int i = 1; i < BUFFER_SIZE; i ++){
        spi_transmit_receive(spi, &send_data, 0, 1, &lsm6dso_rx_buf+i);
    }
    spi_disable_cs(spi, LSM6DS3_CS);

    uint16_t rslt[3];
    rslt[0] = (int16_t)((lsm6dso_rx_buf[IDX_ACCEL_XOUT_H] << 8) | lsm6dso_rx_buf[IDX_ACCEL_XOUT_L]);
    rslt[1] = (int16_t)((lsm6dso_rx_buf[IDX_ACCEL_YOUT_H] << 8) | lsm6dso_rx_buf[IDX_ACCEL_YOUT_L]);
    rslt[2] = (int16_t)((lsm6dso_rx_buf[IDX_ACCEL_ZOUT_H] << 8) | lsm6dso_rx_buf[IDX_ACCEL_ZOUT_L]);
    printf("LSM6DS3 ACC read: X: %i, Y: %i, Z: %i", rslt[0], rslt[1], rslt[2]);
    //acc->ADCRaw[X] = (int16_t)((lsm6dso_rx_buf[IDX_ACCEL_XOUT_H] << 8) | lsm6dso_rx_buf[IDX_ACCEL_XOUT_L]);
    //acc->ADCRaw[Y] = (int16_t)((lsm6dso_rx_buf[IDX_ACCEL_YOUT_H] << 8) | lsm6dso_rx_buf[IDX_ACCEL_YOUT_L]);
    //acc->ADCRaw[Z] = (int16_t)((lsm6dso_rx_buf[IDX_ACCEL_ZOUT_H] << 8) | lsm6dso_rx_buf[IDX_ACCEL_ZOUT_L]);

    return true;
}

bool lsm6dsoGyroRead(SPI_TypeDef *spi)
{
    enum {
        IDX_GYRO_XOUT_L,
        IDX_GYRO_XOUT_H,
        IDX_GYRO_YOUT_L,
        IDX_GYRO_YOUT_H,
        IDX_GYRO_ZOUT_L,
        IDX_GYRO_ZOUT_H,
        BUFFER_SIZE,
    };

    uint8_t lsm6dso_rx_buf[BUFFER_SIZE];
    uint8_t send_data = LSM6DSO_REG_OUTX_L_G;
    spi_enable_cs(spi, LSM6DS3_CS);
    spi_transmit_receive(spi, &send_data, 1, BUFFER_SIZE, &lsm6dso_rx_buf);
    spi_disable_cs(spi, LSM6DS3_CS);
    uint16_t rslt[3];
    rslt[0] = (int16_t)((lsm6dso_rx_buf[IDX_GYRO_XOUT_H] << 8) | lsm6dso_rx_buf[IDX_GYRO_XOUT_L]);
    rslt[1] = (int16_t)((lsm6dso_rx_buf[IDX_GYRO_YOUT_H] << 8) | lsm6dso_rx_buf[IDX_GYRO_YOUT_L]);
    rslt[2] = (int16_t)((lsm6dso_rx_buf[IDX_GYRO_ZOUT_H] << 8) | lsm6dso_rx_buf[IDX_GYRO_ZOUT_L]);
    printf("LSM6DS3 Gryo read: X: %i, Y: %i, Z: %i", rslt[0], rslt[1], rslt[2]);
    //gyro->gyroADCRaw[X] = (int16_t)((lsm6dso_rx_buf[IDX_GYRO_XOUT_H] << 8) | lsm6dso_rx_buf[IDX_GYRO_XOUT_L]);
    //gyro->gyroADCRaw[Y] = (int16_t)((lsm6dso_rx_buf[IDX_GYRO_YOUT_H] << 8) | lsm6dso_rx_buf[IDX_GYRO_YOUT_L]);
    //gyro->gyroADCRaw[Z] = (int16_t)((lsm6dso_rx_buf[IDX_GYRO_ZOUT_H] << 8) | lsm6dso_rx_buf[IDX_GYRO_ZOUT_L]);

    return true;
}