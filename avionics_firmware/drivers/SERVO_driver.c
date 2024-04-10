#include "SERVO_driver.h"


void ServoUartInit(USART_TypeDef* uart){
    UartSingleWireInit(uart, 1000000);
}

void ServoEnable(bool enable){
    gpio_write(_servoEn, enable);
}

SmartServo ServoInit(USART_TypeDef* uart, uint8_t id){
    SmartServo servo;
    servo.servo_uart = uart;
    servo.servo_id = id;

    printf("Initialising Servo ID:%i\r\n", id);
    //ping servo to check its there
    if (ServoPing(&servo)){
        printf("Servo Ping Success\r\n");
    }else{
        printf("Servo ping Failed\r\n");
    }

    //reset servo
    ServoReset(&servo);

    //---------- Configure the servo settings ----------
    delay_milliseconds(10);
    ServoUnlockEeprom(&servo);
    uint8_t send_byte;
    //set respone mode to 0, no response unless requested
    send_byte = 0;
    ServoWriteData(&servo, SERVO_REG_RESPONSE_STATUS_LEVEL, 1, &send_byte);
    //set baud rate
    send_byte = 0;
    ServoWriteData(&servo, SERVO_REG_BAUDRATE, 1, &send_byte);  //baud rate of 1,000,000

    //set min angle
    //0-4096. 15deg = 2048 - (4096/360*15) = 1877
    uint8_t min_angle[2] = {1877 & 0xFF, 1877 >> 8};   //split into 2 bytes, lower section and higher section
    ServoWriteData(&servo, SERVO_REG_MINIMUM_ANGLE, 2, &min_angle);     

    //set max angle
    //0-4096. 15deg = 2048 + (4096/360*15) = 2219
    uint8_t max_angle[2] = {2219 & 0xFF, 2219 >> 8};   //split into 2 bytes, lower section and higher section
    ServoWriteData(&servo, SERVO_REG_MAXIMUM_ANGLE, 2, &max_angle);     

    //set operation mode to position servo
    send_byte = 0;
    ServoWriteData(&servo, SERVO_REG_OPERATION_MODE, 1, &send_byte);
    
    ServoLockEeprom(&servo);
    //---------------------------------------------------
    //set servo to 0 degrees
    ServoSetTargetAngle(&servo, 0);

    return servo;
}

void ServoStartup(SmartServo* servo){
    for (uint8_t s = 0; s < 4; s ++){
        ServoSetTargetAngle(&(servo[s]), -10000);
        delay_milliseconds(200);
        ServoSetTargetAngle(&(servo[s]), 10000);
        delay_milliseconds(200);
        ServoSetTargetAngle(&(servo[s]), 0);
        delay_milliseconds(200);
    }
}

void ServoReset(SmartServo* servo){
    uint8_t txBuf[SERVO_MIN_PACKET_SIZE];    //can't be changed dynamically so set to max
    txBuf[0] = SERVO_HEADER;
    txBuf[1] = SERVO_HEADER;
    txBuf[2] = servo->servo_id;
    txBuf[3] = 0x02;
    txBuf[4] = SERVO_CMD_RESET;

    uint8_t check_sum = ~(txBuf[2] + txBuf[3] + txBuf[4]);  // checksum
    txBuf[5] = check_sum;

    uart_write_buf(servo->servo_uart, txBuf, sizeof(txBuf));    //send buffer

    //============ Recieving data =============
    //delay here
    delay_microseconds(1);

    uint8_t rxBuf[SERVO_MIN_PACKET_SIZE]; //length can't be changed dynamically so set a max
    uint16_t result = 0;
    uint8_t wait_counter;
    uint8_t wait_limit = 250;

    for (uint8_t ii = 0; ii <= sizeof(rxBuf)-1; ii++) {
        wait_counter = 0;
        while(!uart_read_ready(servo->servo_uart))
        {
            wait_counter++;
            delay_microseconds(1); 
            if (wait_counter >= wait_limit){
                servo->servo_error_status = SERVO_ERROR_INVALID_PACKET;
                return false;
            }
        }
        rxBuf[ii] = uart_read_byte(servo->servo_uart);
    }

    //check data?

}

bool ServoPing(SmartServo* servo){
    uint8_t txBuf[SERVO_MIN_PACKET_SIZE];    //can't be changed dynamically so set to max
    txBuf[0] = SERVO_HEADER;
    txBuf[1] = SERVO_HEADER;
    txBuf[2] = servo->servo_id;
    txBuf[3] = 0x02;
    txBuf[4] = SERVO_CMD_PING;

    uint8_t check_sum = ~(txBuf[2] + txBuf[3] + txBuf[4]);  // checksum
    txBuf[5] = check_sum;

    uart_write_buf(servo->servo_uart, txBuf, sizeof(txBuf));    //send buffer

    //============ Recieving data =============
    //delay here
    delay_microseconds(1);

    uint8_t rxBuf[SERVO_MIN_PACKET_SIZE]; //length can't be changed dynamically so set a max
    uint16_t result = 0;
    uint8_t wait_counter;
    uint8_t wait_limit = 250;

    for (uint8_t ii = 0; ii <= sizeof(rxBuf)-1; ii++) {
        wait_counter = 0;

        while(!uart_read_ready(servo->servo_uart))
        {
            wait_counter++;
            delay_microseconds(4); 
            if (wait_counter >= wait_limit){
                servo->servo_error_status = SERVO_ERROR_INVALID_PACKET;
                printf("ping timeout");
                return false;
            }
        }
        rxBuf[ii] = uart_read_byte(servo->servo_uart);
        printf("byte recieved 0x%x", rxBuf[ii]);
    }


    //=========== Process recieved data ==========
    
    //check checksum
    check_sum = 0;
    for (uint8_t i = 2; i < (SERVO_MIN_PACKET_SIZE-1); i++){
        check_sum += rxBuf[i];
    }
    check_sum = ~(check_sum);
    if (check_sum != rxBuf[SERVO_MIN_PACKET_SIZE-1]){
        servo->servo_error_status = SERVO_ERROR_CHECKSUM;
        printf("ping checksum error\r\n");
        return false;
    }

    printf("Ping return = 0x%x\r\n", rxBuf[4]);
    return true;
}

void ServoSetId(SmartServo* servo, uint8_t new_id){
    //check new ID valid
    if (new_id > 0xFE) return;
    //ping oldID
    //if (!ServoPing(servo)) ;//return;
    uint8_t send_byte;
    //unlock EEPROM
    ServoUnlockEeprom(servo);
    //Write new_id to register
    ServoWriteData(servo, SERVO_REG_ID, 1, &new_id);
    //update servo struct
    servo->servo_id = new_id;
    //lock EEPROM
    ServoLockEeprom(servo);
    //check by pinging new_id
    ServoPing(servo);
}

void ServoSetTargetAngle(SmartServo* servo, int32_t target_angle_mdeg){
    int32_t convert_angle_to_position = target_angle_mdeg;
    convert_angle_to_position = (convert_angle_to_position*4096)/360000 + 2048;
    //printf("Position: %i\r\n", convert_angle_to_position);
    ServoSetTargetPosition(servo, convert_angle_to_position);
}

void ServoSetTargetPosition(SmartServo* servo, uint16_t target_postion){
    uint8_t angle_buff[2] = {target_postion & 0xFF, target_postion >> 8};
    ServoWriteData(servo, SERVO_REG_TARGET_POSITION, 2, &angle_buff);
}

void ServoSetPID(SmartServo* servo, uint8_t p, uint8_t i, uint8_t d){
    ServoUnlockEeprom(servo);
    uint8_t send_buff;
    //set p
    if (p != 0xFF){ //set if not equal to 255
        send_buff = p;
        ServoWriteData(servo, SERVO_REG_POS_PROPORTIONAL_GAIN, 1, &send_buff);
    }
    //set i
    if (p != 0xFF){
        send_buff = i;
        ServoWriteData(servo, SERVO_REG_POS_INTEGRAL_GAIN, 1, &send_buff);
    }
    //set d
    if (p != 0xFF){
        send_buff = d;
        ServoWriteData(servo, SERVO_REG_POS_DERIVATIVE_GAIN, 1, &send_buff);
    }
    ServoLockEeprom(servo);
}

void ServoSetOffset(SmartServo* servo, int16_t offset){
    uint8_t send_buff[2];
    if (offset > 0){
        send_buff[0] = offset & 0xFF; //lsb byte
        send_buff[1] = offset & 0xF00; //bits 8 to 11
    }else{
        send_buff[0] = offset & 0xFF; //lsb byte
        send_buff[1] = (offset & 0x700) | 0x800; //bits 8 to 10, with bit 11 as positive to indicate negative direction
    }
    ServoUnlockEeprom(servo);
    ServoWriteData(servo, SERVO_REG_POSITION_CORRECTION, 2, &(send_buff));
    ServoLockEeprom(servo);
}

void ServoSetAccel(SmartServo* servo, uint8_t acccel){
    ServoWriteData(servo, SERVO_REG_TARGET_ACCELERATION,1, &(acccel));
}

int32_t ServoGetCurrentAngle(SmartServo* servo){
    uint16_t position = ServoGetCurrentPosition(servo);
    int32_t angle = ((position - 2048) * 360000) / 4096;
    return angle;
}


uint16_t ServoGetCurrentPosition(SmartServo* servo){
    return ServoReadTwoBytes(servo, SERVO_REG_CURRENT_POSITION);
}

//================== Private functions =======================

void ServoLockEeprom(SmartServo* servo){
    uint8_t send_byte = 1;
    ServoWriteData(servo, SERVO_REG_WRITE_LOCK, 1, &send_byte);
}

void ServoUnlockEeprom(SmartServo* servo){
    uint8_t send_byte = 0;
    ServoWriteData(servo, SERVO_REG_WRITE_LOCK, 1, &send_byte);
}

uint16_t ServoReadTwoBytes(SmartServo* servo, uint8_t address){
    uint8_t result_buff[2] = {0,0};
    ServoReadData(servo, address, 2, &result_buff);
    return result_buff[0] & (result_buff[1] << 8);
}

void ServoWriteData(SmartServo* servo, uint8_t address, uint8_t length, uint8_t* data){
    //check length
    if (length > SERVO_MAX_WRITE_SIZE){
        length = SERVO_MAX_WRITE_SIZE;
    }

    uint8_t txBuf[SERVO_MIN_PACKET_SIZE + SERVO_MAX_WRITE_SIZE];    //can't be changed dynamically so set to max
    txBuf[0] = SERVO_HEADER;
    txBuf[1] = SERVO_HEADER;
    txBuf[2] = servo->servo_id;
    txBuf[3] = length + 3;
    txBuf[4] = SERVO_CMD_WRITE_DATA;
    txBuf[5] = address;

    uint8_t check_sum = txBuf[2] + txBuf[3] + txBuf[4] + txBuf[5];  //first bit of checksum

    //look all data values to add
    for (uint8_t i = 0; i < length; i ++){
        txBuf[i+6] = data[i];   //add to the buffer
        check_sum += data[i];   //add to the checksum
    }
    check_sum = ~(check_sum);
    txBuf[SERVO_MIN_PACKET_SIZE + length] = check_sum;

    uart_write_buf(servo->servo_uart, txBuf, sizeof(txBuf));    //send buffer
}

void ServoReadData(SmartServo* servo, uint8_t address, uint8_t length, uint8_t* result){
    //check length
    if (length > SERVO_MAX_READ_SIZE){
        length = SERVO_MAX_READ_SIZE;
    }

    uint8_t txBuf[SERVO_MIN_PACKET_SIZE + 2];

    txBuf[0] = SERVO_HEADER;
    txBuf[1] = SERVO_HEADER;
    txBuf[2] = servo->servo_id;
    txBuf[3] = 4;
    txBuf[4] = SERVO_CMD_READ_DATA;
    txBuf[5] = address;
    txBuf[6] = length;

    //checksum
    uint8_t check_sum = ~(txBuf[2] + txBuf[3] + txBuf[4] + txBuf[5] + txBuf[6]);
    txBuf[7] = check_sum;

    uart_write_buf(servo->servo_uart, txBuf, sizeof(txBuf));    //send buffer

    //============ Recieving data =============
    //delay here
    delay_microseconds(1);

    uint8_t rxBuf[SERVO_MIN_PACKET_SIZE + SERVO_MAX_READ_SIZE]; //length can't be changed dynamically so set a max
    uint8_t wait_counter;
    uint8_t wait_limit = 250;

    for (uint8_t ii = 0; ii <= sizeof(rxBuf)-1; ii++) {
        wait_counter = 0;
        while(!uart_read_ready(servo->servo_uart))
        {
            wait_counter++;
            delay_microseconds(1); 
            if (wait_counter >= wait_limit){
                servo->servo_error_status = SERVO_ERROR_INVALID_PACKET;
                return;
            }
        }
        rxBuf[ii] = uart_read_byte(servo->servo_uart);
    }


    //=========== Process recieved data ==========
    
    //check checksum
    check_sum = 0;
    for (uint8_t i = 2; i < (SERVO_MIN_PACKET_SIZE + length - 1); i++){
        check_sum += rxBuf[i];
    }
    check_sum = ~(check_sum);
    if (check_sum != rxBuf[SERVO_MIN_PACKET_SIZE + length - 1]){
        servo->servo_error_status = SERVO_ERROR_CHECKSUM;
        return;
    }

    //check error information byte
    if (rxBuf[4] != 0x00){  
        servo->servo_error_status = rxBuf[4];
        return;
    }

    //return results
    for (uint8_t i = 0; i < length; i++){
        result[i] = rxBuf[5+i];
    }
    
}