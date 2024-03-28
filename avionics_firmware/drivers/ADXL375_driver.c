/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Alexandra Posta
    Created on: 15 December 2023
    Description: Driver file for the Accelerometer module ADXL375 (https://www.mouser.co.uk/ProductDetail/Analog-Devices/ADXL375BCCZ?qs=S4ILP0tmc7Q%2Fd%2FHPWf9YpQ%3D%3D)
*/

#include "ADXL375_driver.h"


// Public functions to be called from main flight computer
#pragma region Public
SPI_TypeDef* ADXL375_SPI;

uint8_t ADXL375_init(SPI_TypeDef* spi) {
    // Set up SPI
    ADXL375_SPI = spi;

    // Get the device into 4-wire SPI mode before proceeding
    ADXL375_reg_write(ADXL375_DATA_FORMAT, ADXL375_DATA_FORMAT_SETTINGS(0));

    // Power the CTL Register
    ADXL375_reg_write(ADXL375_POWER_CTL, ADXL375_MEASURE);

    // Check the device name
    spi_enable_cs(ADXL375_CS);
    uint8_t devid;
    uint8_t cmd = ADXL375_DEVID;
    spi_transmit_receive(ADXL375_SPI, &cmd, 1, 1, &devid);
    spi_disable_cs(ADXL375_CS);
    if (devid != ADXL375_DEVID_ID){
        printf("ADXL375 wrong device ID: %d\r\n", devid);
    }
    // Set the data rate
    ADXL375_reg_write(ADXL375_BW_RATE,
			    (0 << ADXL375_BW_RATE_LOW_POWER) |
			    (ADXL375_BW_RATE_RATE_200 << ADXL375_BW_RATE_RATE));

    // Set the offset to zero
    ADXL375_reg_write(ADXL375_OFSX, 0);
    ADXL375_reg_write(ADXL375_OFSY, 0);
    ADXL375_reg_write(ADXL375_OFSZ, 0);   

    // Clear interrupts
	ADXL375_reg_write(ADXL375_INT_ENABLE, 0);

    // Place in measurement mode
    ADXL375_reg_write(ADXL375_POWER_CTL,
			    (0 << ADXL375_POWER_CTL_LINK) |
			    (0 << ADXL375_POWER_CTL_AUTO_SLEEP) |
			    (1 << ADXL375_POWER_CTL_MEASURE) |
			    (0 << ADXL375_POWER_CTL_SLEEP) |
			    (ADXL375_POWER_CTL_WAKEUP_8 << ADXL375_POWER_CTL_WAKEUP)); 


    // Place in FIFO Stream mode
    // FIFO buffer holds the last 32 samples. When the buffer is full, 
    // the oldest data is overwritten with newer data.
    ADXL375_reg_write(ADXL375_FIFO_CTL, ADXL375_FIFO_CTL_MODE_STREAM);

    return 0;
};


/**
  @brief Get x, y, z data from ADXL375
  @param data struct for returning x, y, z data by reference
  @note currently returns raw data, needs to be offset and calibrated in future
  @note if floating points were available, all data would be multiplied by 0.049
  @return 0
*/
uint8_t ADXL375_get_data(ADXL375_data* data){

    // x-axis
    spi_enable_cs(ADXL375_CS);
    delay_miliseconds(1);
    int x_values[2] = {0,0};
    ADXL375_reg_read(ADXL375_X_REG_DATAX0, x_values, 2);
    spi_disable_cs(ADXL375_CS);
    int16_t x = ((uint16_t)x_values[1] << 8) | (uint16_t)x_values[0];
    x = x;

    // y-axis
    spi_enable_cs(ADXL375_CS);
    delay_miliseconds(1);
    int y_values[2] = {0,0};
    ADXL375_reg_read(ADXL375_Y_REG_DATAY0, y_values, 2);
    spi_disable_cs(ADXL375_CS);
    int16_t y = ((uint16_t)y_values[1] << 8) | (uint16_t)y_values[0];
    y = y ;

    // z-axis
    spi_enable_cs(ADXL375_CS);
    delay_miliseconds(1);
    int z_values[2] = {0,0};
    ADXL375_reg_read(ADXL375_Z_REG_DATAZ0, z_values, 2);
    spi_disable_cs(ADXL375_CS);
    int16_t z = ((uint16_t)z_values[1] << 8) | (uint16_t)z_values[0];
    z = (z );

    data->x = x;
    data->y = y;
    data->z = z;

    return 0;
};
#pragma endregion Public

// Implement private functions
#pragma region Private

void ADXL375_reg_write(uint8_t addr, uint8_t value) {
    uint8_t	d[2];
    d[0] = addr;
	d[1] = value;
    uint32_t r;
    spi_enable_cs(ADXL375_CS);
    spi_transmit_receive(ADXL375_SPI, d, 2, 1, &r);
    spi_disable_cs(ADXL375_CS);
}

void ADXL375_reg_read(uint8_t addr, uint8_t *values, int num_val)
{
    int address = addr | 0x80;
    address = address | 0x40;
    spi_enable_cs(ADXL375_CS);
    spi_transmit(ADXL375_SPI, address);
    for(int i = 0; i < num_val; i++)
    {
        values[i] = spi_transmit(ADXL375_SPI, 0x00);
    }
    spi_disable_cs(ADXL375_CS);
}

/*
static void ADXL375_get_test_value(struct ADXL375_data *data, int samples)
{	
	for (int i = 0; i < samples; i++) {
		spi_write_byte(ADXL375_SPI, ADXL375_Z_REG_DATAX0);
        spi_write_byte(ADXL375_SPI, ADXL375_Z_REG_DATAX1);
        spi_ready_read(ADXL375_SPI);

        z_0 = spi_read_byte(ADXL375_SPI, ADXL375_MEASURE);
        z_1 = spi_read_byte(ADXL375_SPI, ADXL375_MEASURE);
   
		data->x += 0;
		data->y += 0;
		data->z += (z_1 << 8) | z_0;
		delay_miliseconds(10);
	}
}
*/
#pragma endregion Private


/* ADD UNDER INIT
// Perform self checks
    struct ADXL375_data	self_test_off, self_test_on;

    ADXL375_reg_write(ADXL375_DATA_FORMAT, ADXL375_DATA_FORMAT_SETTINGS(1)); 

    // Discard some samples to let it settle down
	ADXL375_get_test_value(&self_test_off, ADXL375_SELF_TEST_SETTLE);

	// Get regular values 
	ADXL375_get_test_value(&self_test_off, ADXL375_SELF_TEST_SAMPLES);

    // Turn back to normal mode
    ADXL375_reg_write(ADXL375_DATA_FORMAT, ADXL375_DATA_FORMAT_SETTINGS(0));

    // Discard some samples to let it settle down
	ADXL375_get_test_value(&self_test_off, ADXL375_SELF_TEST_SETTLE);

	// Get regular values 
	ADXL375_get_test_value(&self_test_off, ADXL375_SELF_TEST_SAMPLES);

    // Verify if Z axis values are in range
    int16_t	z_change = self_test_on.z - self_test_off.z;
    self_test_value = z_change;

    // TODO: Check if the self test value is valid


static void ADXL375_start(void) {	
    TO DO: configure chip for: 
        ADXL375_CS_PIN,
        ADXL375_SPI_INDEX,
        ADXL375_SPI_SPEED);
}
*/