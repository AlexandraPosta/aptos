/*
	Leeds University Rocketry Organisation - LURA
    Author Name: Alexandra Posta, Oliver Martin
    Created on: 10 June 2023
    Description: Driver file for the Pressure/Temp/humidity module BME280 
*/

#include "BME280_driver.h"
#include "stdint.h"
#include "mcu.h"

#pragma region Public
int8_t BME280_init(BME280_dev *dev, SPI_TypeDef *spi) {
    int8_t ret_val = 0;
    uint8_t chip_ID = 0;

    dev->BME280_SPI = spi;

    // Read the chip-id of bme280 sensor
    ret_val = BME280_get_regs(BME280_REG_CHIP_ID, &chip_ID, 1, dev);

    // Check for chip id validity
    if (ret_val == 1)
    {
        printf("chip id response: %x", chip_ID);
        if (chip_ID == BME280_CHIP_ID)
        {   
            printf("Got chip ID\r\n");
            dev->chip_ID = chip_ID;
            ret_val = BME280_soft_reset(dev);      // Reset the sensor
            if (ret_val == 1)
            {
                ret_val = get_calib_data(dev);     // Get calibration data
            }
        }else{
            printf("Wrong chipID result\r\n");
        }
    }else{
        printf("Error getting chipID\r\n");
    }
    return ret_val;
};


int8_t BME280_soft_reset(BME280_dev *dev)
{
    int8_t ret_val;
    uint8_t reg_addr = BME280_REG_RESET;
    uint8_t status_reg = 0;
    uint8_t try_run = 5;
    uint8_t soft_rst_cmd = BME280_CMD_SOFT_RESET; // 0xB6

    // Write the soft reset command in the sensor
    ret_val = BME280_set_regs(&reg_addr, &soft_rst_cmd, 1, dev);

    if (ret_val == 1)
    {
        do
        {
            // As per data sheet - Table 1, startup time is 2 ms
            delay_ms(BME280_STARTUP_DELAY/1000);
            ret_val = BME280_get_regs(BME280_REG_STATUS, &status_reg, 1, dev);

        } while ((ret_val == 1) && (try_run--) && (status_reg & BME280_CMD_STATUS_IM_UPDATE));

        if (status_reg & BME280_CMD_STATUS_IM_UPDATE)
        {
            ret_val = BME280_E_NVM_COPY_FAILED;
        }
    }
}


int8_t BME280_get_regs(uint8_t reg_addr, uint8_t *reg_data, uint16_t len, BME280_dev *dev)
{
    int8_t ret_val;
    ret_val = null_ptr_check(dev); // Check for null pointer in the device structure

    if ((ret_val == 1) && (reg_data != NULL))
    {        
        reg_addr = reg_addr | 0x80; // SPI
        printf("REG ADDRESS %x\r\n", reg_addr);
        //printf("Sending data to get register\r\n");
        
        //spi_transmit_receive(dev->BME280_SPI, BME280_CS, reg_addr, 1, len, reg_data); //SPI READ
        
        for (uint8_t i = 0; i < len; i ++){
            spi_enable_cs(dev->BME280_SPI, BME280_CS);
            spi_transmit(dev->BME280_SPI, reg_addr+i);
            reg_data[i] = spi_read_byte(dev->BME280_SPI);
            spi_disable_cs(dev->BME280_SPI, BME280_CS);
        }
        
        
        //spi_read_buf(dev->BME280_SPI, reg_data, len);
        
        //dev->intf_rslt = reg_data;
        //dev->intf_rslt = dev->read(reg_addr, reg_data, len);   // Read the data ****Replace this line with spi_transmit_receive()

        // Check for communication error
        if (dev->intf_rslt != BME280_INTF_RET_SUCCESS)
        {
            ret_val = BME280_E_COMM_FAIL;
        }
    }else{
        printf("Get reg error - 1\r\n");
    }
    return ret_val;
}


int8_t BME280_set_regs(uint8_t *reg_addr, const uint8_t *reg_data, uint16_t len, BME280_dev *dev)
{
    int8_t ret_val;
    uint8_t temp_buff[20]; // Typically not to write more than 10 registers
    uint16_t temp_len;
    uint16_t reg_addrCnt;

    if (len > 10) // max allowed length is 10
    {
        len = 10;
    }

    // Check for null pointer in the device structure 
    ret_val = null_ptr_check(dev);

    // Check for arguments validity 
    if ((ret_val == 1) && (reg_addr != NULL) && (reg_data != NULL))
    {
        if (len != 0)
        {
            temp_buff[0] = reg_data[0];

            // SPI
            for (reg_addrCnt = 0; reg_addrCnt < len; reg_addrCnt++)
            {
                reg_addr[reg_addrCnt] = reg_addr[reg_addrCnt] & 0x7F;
            }

            // Burst write mode
            if (len > 1)
            {
                // Interleave register address w.r.t data for burst write
                interleave_reg_addr(reg_addr, temp_buff, reg_data, len);
                temp_len = ((len * 2) - 1);
            } else { 
                temp_len = len;
            }
            
            //figure out what data needs to be sent
            printf("Sending data to set register\r\n");
            spi_enable_cs(dev->BME280_SPI, BME280_CS);
            spi_transmit_receive(dev->BME280_SPI, temp_buff, temp_len, 1,  &dev->intf_rslt);
            spi_disable_cs(dev->BME280_SPI, BME280_CS);
            //dev->intf_rslt = spi_transmit_receive(BME280_SPI, BME280_CS, temp_buff, temp_len, 1);
            //dev->intf_rslt = dev->write(reg_addr[0], temp_buff, temp_len, dev->intf_rslt); //****Replace this line with spi_transmit_receive()
        }
    }else{
        printf("Set reg error\r\n");
    }
    return ret_val;
}


int8_t BME280_get_data(uint8_t sensor_comp, BME280_data *compData, BME280_dev *dev)
{
    int8_t ret_val = 0;

    // Array to store the pressure, temperature and humidity data 
    uint8_t reg_data[BME280_LEN_P_T_H_DATA] = { 0 };
    BME280_uncomp_data uncomp_data = { 0 };

    if (compData != NULL)
    {
        /* Read the pressure and temperature data from the sensor */
        ret_val = BME280_get_regs(BME280_REG_DATA, &reg_data, BME280_LEN_P_T_H_DATA, dev);

        if (ret_val == 1)
        {
            // Parse the read data from the sensor 
            parse_sensor_data(&reg_data, &uncomp_data);

            // Compensate the pressure and/or temperature and/or humidity data from the sensor
            ret_val = BME280_compensate_data(sensor_comp, &uncomp_data, compData, &(dev->calib));
        }
    }
    return ret_val;
}


int8_t BME280_compensate_data(uint8_t sensor_comp, const BME280_uncomp_data *uncomp_data,
                              BME280_data *compData, BME280_calib_data *calib_data)
{
    int8_t ret_val = 1;

    if ((uncomp_data != NULL) && (compData != NULL) && (compData != NULL))
    {
        // Initialize to zero
        compData->temperature = 0;
        compData->pressure = 0;
        compData->humidity = 0;

        // If pressure or temperature component is selected 
        if (sensor_comp & (BME280_PRESS | BME280_TEMP | BME280_HUM))
        {
            // Compensate the temperature data 
            compData->temperature = compensate_temperature(uncomp_data, calib_data);
        }

        if (sensor_comp & BME280_PRESS)
        {
            // Compensate the pressure data
            compData->pressure = compensate_pressure(uncomp_data, calib_data);
        }

        if (sensor_comp & BME280_HUM)
        {
            // Compensate the humidity data
            compData->humidity = compensate_humidity(uncomp_data, calib_data);
        }
    }
    else
    {
        ret_val = BME280_E_NULL_PTR;
    }

    return ret_val;
}
#pragma endregion Public

#pragma region Private
/* ---------------------------------- Private Functions ---------------------------------- */
/**
 @brief This private function is used to validate the device structure pointer for
 null conditions.
*/
int8_t null_ptr_check(BME280_dev *dev)
{
    int8_t ret_val;

    if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL))
    {
        // Device structure pointer is not valid 
        ret_val = BME280_E_NULL_PTR;
        printf("BME280 NULL Pointer Error.\r\n");
    }
    else
    {
        // Device structure is fine 
        ret_val = 1;
    }

    return ret_val;
}


/**
 @brief This private function reads the calibration data from the sensor, parse
 it and store in the device structure.
*/
int8_t get_calib_data(BME280_dev *dev)
{
    int8_t ret_val;
    uint8_t reg_addr = BME280_REG_TEMP_PRESS_CALIB_DATA;

    /* Array to store calibration data */
    uint8_t calib_data[BME280_LEN_TEMP_PRESS_CALIB_DATA] = { 0 };

    /* Read the calibration data from the sensor */
    ret_val = BME280_get_regs(reg_addr, &calib_data, BME280_LEN_TEMP_PRESS_CALIB_DATA, dev);

    if (ret_val == 1)
    {
        // Parse temperature and pressure calibration data and store it in device structure
        parse_temp_press_calib_data(&calib_data, dev);
        reg_addr = BME280_REG_HUMIDITY_CALIB_DATA;

        // Read the humidity calibration data from the sensor
        ret_val = BME280_get_regs(reg_addr, &calib_data, BME280_LEN_HUMIDITY_CALIB_DATA, dev);

        if (ret_val == 1)
        {
            // Parse humidity calibration data and store in device structure
            parse_humidity_calib_data(&calib_data, dev);
        }
    }

    return ret_val;
}

/*!
 *  @brief This API is used to parse the pressure, temperature and
 *  humidity data and store it in the bme280_uncomp_data structure instance.
 */
static void parse_sensor_data(const uint8_t *reg_data, struct BME280_uncomp_data *uncomp_data)
{
    /* Variables to store the sensor data */
    uint32_t data_xlsb;
    uint32_t data_lsb;
    uint32_t data_msb;

    /* Store the parsed register values for pressure data */
    data_msb = (uint32_t)reg_data[0] << 12;
    data_lsb = (uint32_t)reg_data[1] << 4;
    data_xlsb = (uint32_t)reg_data[2] >> 4;
    uncomp_data->pressure = data_msb | data_lsb | data_xlsb;

    /* Store the parsed register values for temperature data */
    data_msb = (uint32_t)reg_data[3] << 12;
    data_lsb = (uint32_t)reg_data[4] << 4;
    data_xlsb = (uint32_t)reg_data[5] >> 4;
    uncomp_data->temperature = data_msb | data_lsb | data_xlsb;

    /* Store the parsed register values for humidity data */
    data_msb = (uint32_t)reg_data[6] << 8;
    data_lsb = (uint32_t)reg_data[7];
    uncomp_data->humidity = data_msb | data_lsb;
}

/**
 @brief This private function is used to parse the temperature and
 pressure calibration data and store it in device structure.
*/
void parse_temp_press_calib_data(const uint8_t *reg_data, BME280_dev *dev)
{
    BME280_calib_data *calib_data = &(dev->calib);

    calib_data->dig_t1 = BME280_CONCAT_BYTES(reg_data[1], reg_data[0]);
    calib_data->dig_t2 = (int16_t)BME280_CONCAT_BYTES(reg_data[3], reg_data[2]);
    calib_data->dig_t3 = (int16_t)BME280_CONCAT_BYTES(reg_data[5], reg_data[4]);
    calib_data->dig_p1 = BME280_CONCAT_BYTES(reg_data[7], reg_data[6]);
    calib_data->dig_p2 = (int16_t)BME280_CONCAT_BYTES(reg_data[9], reg_data[8]);
    calib_data->dig_p3 = (int16_t)BME280_CONCAT_BYTES(reg_data[11],reg_data[10]);
    calib_data->dig_p4 = (int16_t)BME280_CONCAT_BYTES(reg_data[13],reg_data[12]);
    calib_data->dig_p5 = (int16_t)BME280_CONCAT_BYTES(reg_data[15],reg_data[14]);
    calib_data->dig_p6 = (int16_t)BME280_CONCAT_BYTES(reg_data[17],reg_data[16]);
    calib_data->dig_p7 = (int16_t)BME280_CONCAT_BYTES(reg_data[19],reg_data[18]);
    calib_data->dig_p8 = (int16_t)BME280_CONCAT_BYTES(reg_data[21],reg_data[20]);
    calib_data->dig_p9 = (int16_t)BME280_CONCAT_BYTES(reg_data[23],reg_data[22]);
    calib_data->dig_h1 = reg_data[25];
}


/**
 @brief This private function is used to parse the humidity calibration data
 and store it in device structure.
*/
void parse_humidity_calib_data(const uint8_t *reg_data, BME280_dev* dev)
{
    struct BME280_calib_data* calib_data = &(dev->calib);
    int16_t dig_h4_lsb;
    int16_t dig_h4_msb;
    int16_t dig_h5_lsb;
    int16_t dig_h5_msb;

    calib_data->dig_h2 = (int16_t)BME280_CONCAT_BYTES(reg_data[1], reg_data[0]);
    calib_data->dig_h3 = reg_data[2];
    dig_h4_msb = (int16_t)(int8_t)reg_data[3] * 16;
    dig_h4_lsb = (int16_t)(reg_data[4] & 0x0F);
    calib_data->dig_h4 = dig_h4_msb | dig_h4_lsb;
    dig_h5_msb = (int16_t)(int8_t)reg_data[5] * 16;
    dig_h5_lsb = (int16_t)(reg_data[4] >> 4);
    calib_data->dig_h5 = dig_h5_msb | dig_h5_lsb;
    calib_data->dig_h6 = (int8_t)reg_data[6];
}


/**
    @brief This private function interleaves the register address between the
    register data buffer for burst write operation.
*/
void interleave_reg_addr(const uint8_t *reg_addr, uint8_t *tempBuff, const uint8_t *reg_data, uint16_t len)
{
    uint16_t index;

    for (index = 1; index < len; index++)
    {
        tempBuff[(index * 2) - 1] = reg_addr[index];
        tempBuff[index * 2] = reg_data[index];
    }
}


/**
    @brief This private function is used to compensate the raw temperature data and
    return the compensated temperature data in integer data type.
*/
int32_t compensate_temperature(const BME280_uncomp_data *uncomp_data, BME280_calib_data *calib_data)
{
    int32_t var1;
    int32_t var2;
    int32_t temperature;
    int32_t temperature_min = -4000;
    int32_t temperature_max = 8500;

    var1 = (int32_t)((uncomp_data->temperature / 8) - ((int32_t)calib_data->dig_t1 * 2));
    var1 = (var1 * ((int32_t)calib_data->dig_t2)) / 2048;
    var2 = (int32_t)((uncomp_data->temperature / 16) - ((int32_t)calib_data->dig_t1));
    var2 = (((var2 * var2) / 4096) * ((int32_t)calib_data->dig_t3)) / 16384;
    calib_data->t_fine = var1 + var2;
    temperature = (calib_data->t_fine * 5 + 128) / 256;

    if (temperature < temperature_min)
    {
        temperature = temperature_min;
    } 
    else if (temperature > temperature_max)
    {
        temperature = temperature_max;
    }
    return temperature;
}


/**
    @brief This private function is used to compensate the raw pressure data and
    return the compensated pressure data in integer data type with high accuracy.
*/
uint32_t compensate_pressure(const BME280_uncomp_data *uncomp_data, const BME280_calib_data *calib_data)
{
    int64_t var1;
    int64_t var2;
    int64_t var3;
    int64_t var4;
    uint32_t pressure;
    uint32_t pressure_min = 3000000;
    uint32_t pressure_max = 11000000;

    var1 = ((int64_t)calib_data->t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib_data->dig_p6;
    var2 = var2 + ((var1 * (int64_t)calib_data->dig_p5) * 131072);
    var2 = var2 + (((int64_t)calib_data->dig_p4) * 34359738368);
    var1 = ((var1 * var1 * (int64_t)calib_data->dig_p3) / 256) + ((var1 * ((int64_t)calib_data->dig_p2) * 4096));
    var3 = ((int64_t)1) * 140737488355328;
    var1 = (var3 + var1) * ((int64_t)calib_data->dig_p1) / 8589934592;

    /* To avoid divide by zero exception */
    if (var1 != 0)
    {
        var4 = 1048576 - uncomp_data->pressure;
        var4 = (((var4 * INT64_C(2147483648)) - var2) * 3125) / var1;
        var1 = (((int64_t)calib_data->dig_p9) * (var4 / 8192) * (var4 / 8192)) / 33554432;
        var2 = (((int64_t)calib_data->dig_p8) * var4) / 524288;
        var4 = ((var4 + var1 + var2) / 256) + (((int64_t)calib_data->dig_p7) * 16);
        pressure = (uint32_t)(((var4 / 2) * 100) / 128);

        if (pressure < pressure_min)
        {
            pressure = pressure_min;
        }
        else if (pressure > pressure_max)
        {
            pressure = pressure_max;
        }
    } else {
        pressure = pressure_min;
    }
    return pressure;
}


/**
    @brief This internal API is used to compensate the raw humidity data and
    return the compensated humidity data in integer data type.
*/
uint32_t compensate_humidity(const BME280_uncomp_data *uncomp_data, const BME280_calib_data *calib_data)
{
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    uint32_t humidity;
    uint32_t humidity_max = 102400;

    var1 = calib_data->t_fine - ((int32_t)76800);
    var2 = (int32_t)(uncomp_data->humidity * 16384);
    var3 = (int32_t)(((int32_t)calib_data->dig_h4) * 1048576);
    var4 = ((int32_t)calib_data->dig_h5) * var1;
    var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t)calib_data->dig_h6)) / 1024;
    var3 = (var1 * ((int32_t)calib_data->dig_h3)) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t)calib_data->dig_h2)) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t)calib_data->dig_h1)) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);
    humidity = (uint32_t)(var5 / 4096);

    if (humidity > humidity_max)
    {
        humidity = humidity_max;
    }

    return humidity;
}

/*!
 *  @brief Prints the execution status of the APIs.
 */
void BME280_error_codes_print_result(const char api_name[], int8_t rslt)
{
    if (rslt != BME280_OK)
    {
        printf("%s\t", api_name);

        switch (rslt)
        {
            case BME280_E_NULL_PTR:
                printf("Error [%d] : Null pointer error.", rslt);
                printf(
                    "It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL.\r\n");
                break;

            case BME280_E_COMM_FAIL:
                printf("Error [%d] : Communication failure error.", rslt);
                printf(
                    "It occurs due to read/write operation failure and also due to power failure during communication\r\n");
                break;

            case BME280_E_DEV_NOT_FOUND:
                printf("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read\r\n",
                       rslt);
                break;
            default:
                printf("Error [%d]\r\n", rslt);
        }
    }
}


#pragma endregion Private
