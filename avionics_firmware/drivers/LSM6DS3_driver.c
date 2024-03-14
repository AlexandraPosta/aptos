/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Oliver Martin
    Created on: 01 March 2024
    Description: Driver file for the IMU module LSM6DS3 (https://www.mouser.co.uk/datasheet/2/389/dm00133076-1798402.pdf)

    ---------------------------------------
    Some basis for this logic comes from the open source Betaflight lsm6dso driver.

    TODO:
        Low pass filter
        Notch filter as well?

*/

#include "LSM6DS3_driver.h"

uint8_t lsm6ds3_init(SPI_TypeDef *spi, LSM6DS3_data* gyro)
{
    uint8_t chipID = 0;
    uint8_t send_data[1] = {(LSM6DSO_REG_WHO_AM_I) | 0x80}; 
    spi_enable_cs(spi, LSM6DS3_CS);
    delay_microseconds(1);
    //spi_transmit_receive(spi, LSM6DSO_REG_WHO_AM_I, &chipID, 1, 1, &rslt);
    spi_transmit_receive(spi, &send_data, 1, 1, &chipID);
    delay_microseconds(1);
    spi_disable_cs(spi, LSM6DS3_CS);

    if (chipID == LSM6DS3_WHO_AM_I_EXP) {
        printf("LSM6DS3 Correct ChipID\r\n");
        delay_microseconds(10);
        lsm6ds3Config(SPI2);
        delay_microseconds(10);

        gyro->x = 0;
        gyro->y = 0;
        gyro->z = 0;
        gyro->pitch = 0;
        gyro->roll = 0;
        gyro->yaw = 0;
        gyro->xOffset = 0;
        gyro->yOffset = 0;
        gyro->zOffset = 0;
        lsm6ds3GyroOffsets(spi, gyro);  // calculate gyro offsets
        gyro->time = get_time_us();
        return 1;
    }else{
        printf("LSM6DS3: Incorrect ChipID\r\n");
    }
    
    return 0;
}

static void lsm6ds3WriteRegister(SPI_TypeDef *spi, uint8_t registerID, uint8_t value, unsigned delayMs)
{
    uint8_t send_data[2] =  {registerID, value};
    spi_enable_cs(spi, LSM6DS3_CS);
    delay_microseconds(10);
    spi_transmit_receive(spi, &(send_data[0]), 1, 0, &send_data);
    spi_transmit_receive(spi, &(send_data[1]), 1, 0, &send_data);
    delay_microseconds(1);
    spi_disable_cs(spi, LSM6DS3_CS);
    if (delayMs) {
        delay_ms(delayMs);
    }
}

static void lsm6ds3WriteRegisterBits(SPI_TypeDef *spi, uint8_t registerID, uint8_t mask, uint8_t value, unsigned delayMs)
{
    uint8_t newValue = 0;
    spi_enable_cs(spi, LSM6DS3_CS);
    delay_microseconds(10);
    uint8_t send_data = registerID | 80;
    spi_transmit_receive(spi, &send_data, 1, 1, &newValue); //get current data
    delay_microseconds(1);
    spi_disable_cs(spi, LSM6DS3_CS);
    
    delay_microseconds(delayMs);
    newValue = (newValue & ~mask) | value;
    lsm6ds3WriteRegister(spi, registerID, newValue, delayMs);
    delay_microseconds(delayMs);
}

void lsm6ds3Config(SPI_TypeDef *spi){
    // Reset the device (wait 100ms before continuing config)
    lsm6ds3WriteRegisterBits(spi, LSM6DSO_REG_CTRL3_C, LSM6DSO_MASK_CTRL3_C_RESET, BIT(0), 100);
    delay_ms(100);
    // Configure interrupt pin 1 for gyro data ready only
    //lsm6ds3WriteRegister(spi, LSM6DSO_REG_INT1_CTRL, LSM6DSO_VAL_INT1_CTRL, 1);

    // Disable interrupt pin 2
    //lsm6ds3WriteRegister(spi, LSM6DSO_REG_INT2_CTRL, LSM6DSO_VAL_INT2_CTRL, 1);

    // Configure the accelerometer
    // 833hz ODR, 16G scale, use LPF1 output
    lsm6ds3WriteRegister(spi, LSM6DSO_REG_CTRL1_XL, (LSM6DSO_VAL_CTRL1_XL_ODR833 << 4) | (LSM6DSO_VAL_CTRL1_XL_16G << 2) | (LSM6DSO_VAL_CTRL1_XL_LPF1 << 1), 1);

    // Configure the gyro
    // 6664hz ODR, 2000dps scale
    lsm6ds3WriteRegister(spi, LSM6DSO_REG_CTRL2_G, (LSM6DSO_VAL_CTRL2_G_ODR6664 << 4) | (LSM6DSO_VAL_CTRL2_G_2000DPS << 2), 1);

    // Configure control register 3
    // latch LSB/MSB during reads; set interrupt pins active high; set interrupt pins push/pull; set 4-wire SPI; enable auto-increment burst reads
    lsm6ds3WriteRegisterBits(spi, LSM6DSO_REG_CTRL3_C, LSM6DSO_MASK_CTRL3_C, (LSM6DSO_VAL_CTRL3_C_BDU | LSM6DSO_VAL_CTRL3_C_H_LACTIVE | LSM6DSO_VAL_CTRL3_C_PP_OD | LSM6DSO_VAL_CTRL3_C_SIM | LSM6DSO_VAL_CTRL3_C_IF_INC), 1);

    // Configure control register 4
    // enable accelerometer high performane mode; set gyro LPF1 cutoff to 335.5hz
    lsm6ds3WriteRegisterBits(spi, LSM6DSO_REG_CTRL4_C, LSM6DSO_MASK_CTRL4_C, (LSM6DSO_VAL_CTRL4_C_I2C_DISABLE | LSM6DSO_VAL_CTRL4_C_LPF1_SEL_G), 1);

    // Configure control register 6
    // disable I2C interface; enable gyro LPF1
    lsm6ds3WriteRegisterBits(spi, LSM6DSO_REG_CTRL6_C, LSM6DSO_MASK_CTRL6_C, (LSM6DSO_VAL_CTRL6_C_XL_HM_MODE | LSM6DSO_VAL_CTRL6_C_FTYPE_335HZ), 1);

    // Configure control register 9
    // disable I3C interface
    lsm6ds3WriteRegisterBits(spi, LSM6DSO_REG_CTRL9_XL, LSM6DSO_MASK_CTRL9_XL, LSM6DSO_VAL_CTRL9_XL_I3C_DISABLE, 1);
    return;
}

/*
void lsm6ds3ExtiHandler(extiCallbackRec_t *cb)
{
    gyroDev_t *gyro = container_of(cb, gyroDev_t, exti);
    gyro->dataReady = true;
}*/

bool lsm6ds3AccRead(SPI_TypeDef *spi, LSM6DS3_data* gyro)
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

    uint8_t lsm6ds3_rx_buf[BUFFER_SIZE];
    uint8_t send_data =  LSM6DSO_REG_OUTX_L_A | 0x80;

    spi_enable_cs(spi, LSM6DS3_CS);
    delay_microseconds(1);
    spi_transmit_receive(spi, &(send_data), 1, 1, &(lsm6ds3_rx_buf[0]));  //send read command and get first result
    
    for (int i = 1; i < BUFFER_SIZE; i ++){
        //send_data = (LSM6DSO_REG_OUTX_L_A+i) | 0x80;
        spi_transmit_receive(spi, &(send_data), 0, 1, &(lsm6ds3_rx_buf[i]));
    } 
    delay_microseconds(1);
    spi_disable_cs(spi, LSM6DS3_CS);

    gyro->xAccel = ((lsm6ds3_rx_buf[IDX_ACCEL_XOUT_H] << 8) | lsm6ds3_rx_buf[IDX_ACCEL_XOUT_L]);
    gyro->yAccel = ((lsm6ds3_rx_buf[IDX_ACCEL_YOUT_H] << 8) | lsm6ds3_rx_buf[IDX_ACCEL_YOUT_L]);
    gyro->zAccel = ((lsm6ds3_rx_buf[IDX_ACCEL_ZOUT_H] << 8) | lsm6ds3_rx_buf[IDX_ACCEL_ZOUT_L]);
    //printf("Accel: X:%6i, \tY:%6i,\tZ:%6i\r\n", gyro->xAccel, gyro->yAccel, gyro->zAccel);

    return true;
}

bool lsm6ds3GyroRead(SPI_TypeDef *spi, LSM6DS3_data* gyro)
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

    uint8_t lsm6ds3_rx_buf[BUFFER_SIZE] = {0};
    uint8_t send_data = (LSM6DSO_REG_OUTX_L_G) | 0x80;

    spi_enable_cs(spi, LSM6DS3_CS);
    delay_microseconds(1);
    spi_transmit_receive(spi, &(send_data), 1, 1, &(lsm6ds3_rx_buf[0])); //send read command and get first result
    for (uint8_t i = 1; i < BUFFER_SIZE; i ++){
        //send_data = (LSM6DSO_REG_OUTX_L_G+i) | 0x80;
        spi_transmit_receive(spi, &(send_data), 0, 1, &(lsm6ds3_rx_buf[i]));
    }
    delay_microseconds(1);
    spi_disable_cs(spi, LSM6DS3_CS);
    gyro->xRate = LMS6DS6_ANGULAR_RATE_SENSITIVITY * (int32_t)((int16_t)((lsm6ds3_rx_buf[IDX_GYRO_XOUT_H] << 8) | lsm6ds3_rx_buf[IDX_GYRO_XOUT_L])) - gyro->xOffset;
    gyro->yRate = LMS6DS6_ANGULAR_RATE_SENSITIVITY * (int32_t)((int16_t)((lsm6ds3_rx_buf[IDX_GYRO_YOUT_H] << 8) | lsm6ds3_rx_buf[IDX_GYRO_YOUT_L])) - gyro->yOffset;
    gyro->zRate = LMS6DS6_ANGULAR_RATE_SENSITIVITY * (int32_t)((int16_t)((lsm6ds3_rx_buf[IDX_GYRO_ZOUT_H] << 8) | lsm6ds3_rx_buf[IDX_GYRO_ZOUT_L])) - gyro->zOffset;
    
    //printf("GryoR: X:%i, \tY:%i,\tZ:%i\r\n", gyro->xRate, gyro->yRate, gyro->zRate);

    return true;
}
//get IMU accelerometer values
bool lsm6ds3GyroReadAngle(SPI_TypeDef *spi, LSM6DS3_data* gyro)
{
    //first reading
    lsm6ds3GyroRead(spi, gyro);
    int32_t dx = gyro->xRate;  //mdeg/s
    int32_t dy = gyro->yRate;
    int32_t dz = gyro->zRate;

    //subsequent readings for downsampling
    for (uint8_t i = 1; i < LSM6DS6_DOWNSAMPLE_SIZE; i ++){
        lsm6ds3GyroRead(spi, gyro);
        dx += gyro->xRate;  //mdeg/s
        dy += gyro->yRate;
        dz += gyro->zRate;
        delay_microseconds(1);
    }

    int32_t currentTime = get_time_us() & 0x7FFFFFFF; //convert time to signed number but don't let it be negative.
    int32_t dt = (currentTime - gyro->time);

    //calculate downsample value and then integrate.
    dx = (dx/LSM6DS6_DOWNSAMPLE_SIZE) *dt;
    dy = (dy/LSM6DS6_DOWNSAMPLE_SIZE) *dt;
    dz = (dz/LSM6DS6_DOWNSAMPLE_SIZE) *dt;
    
    //printf("Gryo d: X:%6i, \tY:%6i,\tZ:%6i\r\n", dx, dy, dz);
    dx = dx/1000000;
    dy = dy/1000000;
    dz = dz/1000000;

    //add angle change
    gyro->x = LSM6DS3_angle_overflow(gyro->x + dx);  
    gyro->y = LSM6DS3_angle_overflow(gyro->y + dy);
    gyro->z = LSM6DS3_angle_overflow(gyro->z + dz);
    
    gyro->time = currentTime;
    printf("GryoA: X:%6i, \tY:%6i,\tZ:%6i\r\n", gyro->x/100, gyro->y/100, gyro->z/100);
    return 1;
}

// Function to calculate orientation using complementary filter
void lsm6ds3CalculateOrientation(SPI_TypeDef *spi, LSM6DS3_data* gyro) {

    //first reading
    lsm6ds3GyroRead(spi, gyro);
    
    int32_t dx = gyro->xRate;  //mdeg/s
    int32_t dy = gyro->yRate;
    int32_t dz = gyro->zRate;
    /*
    //subsequent readings for downsampling
    for (uint8_t i = 1; i < LSM6DS6_DOWNSAMPLE_SIZE; i ++){
        lsm6ds3GyroRead(spi, gyro);
        dx += gyro->xRate;  //mdeg/s
        dy += gyro->yRate;
        dz += gyro->zRate;
        delay_microseconds(1);
    }
    */
    //time 
    int32_t currentTime = get_time_us() & 0x7FFFFFFF; //convert time to signed number but don't let it be negative.
    int32_t dt = (currentTime - gyro->time);
    printf("DT:%i", dt);
    gyro->time = currentTime;

    //calculate downsample value and then integrate.
    dx = (dx/LSM6DS6_DOWNSAMPLE_SIZE) *dt;
    dy = (dy/LSM6DS6_DOWNSAMPLE_SIZE) *dt;
    dz = (dz/LSM6DS6_DOWNSAMPLE_SIZE) *dt;

    //read accelerometer
    lsm6ds3AccRead(spi, gyro);

    // Convert accelerometer data to roll and pitch angles
    int32_t accelRoll = atan2_fixed(gyro->yAccel, gyro->xAccel);
    int32_t accelPitch = atan2_fixed(-gyro->xAccel, sqrt_fixed((gyro->yAccel * gyro->yAccel)>>6 + (gyro->zAccel * gyro->zAccel)>>6));


    int32_t gyroRoll = (gyro->xRate * dt);
    gyroRoll = gyroRoll/1000000;
    int32_t gyroPitch = (gyro->yRate * dt);
    gyroPitch = gyroPitch/1000000;

    // Integrate gyro data for yaw
    gyro->yaw += ((gyro->zRate) * dt)/1000000; // dt is the time difference between sensor updates

    // Apply complementary filter
    gyro->roll = LSM6DS3_angle_overflow(ACCEL_WEIGHT * (accelRoll) + GYRO_WEIGHT * (gyro->roll + gyroRoll));
    gyro->pitch = LSM6DS3_angle_overflow(ACCEL_WEIGHT * (accelPitch) + GYRO_WEIGHT * (gyro->pitch + gyroPitch));
    //keeps angle between +-180,000 mDeg

    printf("P:%6i, \tR:%6i,\tY:%6i\r\n", gyro->pitch/1000, gyro->roll/1000, gyro->yaw/1000);
}

//calculates the gyro offset values
bool lsm6ds3GyroOffsets(SPI_TypeDef *spi, LSM6DS3_data* gyro)
{
    LSM6DS3_data buff[LSM6DSO_OFFSET_BUFF_LEN];
    int32_t avg[3] = {0,0,0};
    lsm6ds3GyroRead(spi, gyro);
    delay_ms(300);
    for (uint8_t i = 0; i < LSM6DSO_OFFSET_BUFF_LEN; i++){
        lsm6ds3GyroRead(spi, gyro);
        buff[i] = *gyro;
        avg[0] += buff[i].xRate;
        avg[1] += buff[i].yRate;
        avg[2] += buff[i].zRate;
        //printf("Offset Sums: %i, %i, %i\r\n", avg[0], avg[1], avg[2]);
        delay_microseconds(1000000/100);
    }
    gyro->xOffset = (avg[0] / LSM6DSO_OFFSET_BUFF_LEN);
    gyro->yOffset = (avg[1] / LSM6DSO_OFFSET_BUFF_LEN);
    gyro->zOffset = (avg[2] / LSM6DSO_OFFSET_BUFF_LEN);
    printf("Gyro Offsets: %i, %i, %i\r\n", gyro->xOffset, gyro->yOffset, gyro->zOffset);

    return 1;
}

//keeps angle between +-180,000 mDeg
int32_t LSM6DS3_angle_overflow(int32_t mDeg){
    if (mDeg > 180000){
        return mDeg - 360000;
    }else if (mDeg < -180000){
        return mDeg + 360000;
    }
    return mDeg;
}