#include "SERVO_driver.h"





SmartServo ServoInit(USART_TypeDef* uart, uint8_t id){
    SmartServo servo;
    servo.servo_uart = uart;
    servo.servo_id = id;

    //ping servo to check its there
    if (ServoPing(&servo))
    //reset servo
    ServoReset(&servo);

    //set servo to 0 degrees

    return servo;
}

void ServoReset(SmartServo* servo){

}

bool ServoPing(SmartServo* servo){
    uint8_t txBuf[SERVO_MIN_PACKET_SIZE];    //can't be changed dynamically so set to max
    txBuf[0] = SERVO_HEADER;
    txBuf[1] = SERVO_HEADER;
    txBuf[2] = servo->servo_id;
    txBuf[3] = 0x02;
    txBuf[4] = SERVO_CMD_PING;

    uint8_t check_sum = ~(txBuf[2] + txBuf[3] + txBuf[4] + txBuf[5]);  // checksum
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


    //=========== Process recieved data ==========
    
    //check checksum
    check_sum = 0;
    for (uint8_t i = 2; i < (SERVO_MIN_PACKET_SIZE); i++){
        check_sum += rxBuf[i];
    }
    check_sum = ~(check_sum);
    if (check_sum != rxBuf[SERVO_MIN_PACKET_SIZE-1]){
        servo->servo_error_status = SERVO_ERROR_CHECKSUM;
        return false;
    }

    return true;
}

//================== Private functions =======================

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
                return 0;
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