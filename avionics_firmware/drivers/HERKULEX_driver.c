
#include "HERKULEX_driver.h"

void HERKULEX_init(SmartServo *motor)
{
    //Any Uart initialisation needed?

    //-------- Starting sequence -------------

    //Do factory reset to avoid any issues
    //set ACK policy in ROM to 1 (reply only when read CMD sent)

    HERKULEX_servo_set_torque(motor, SERVO_TORQUE_ON);
    HERKULEX_position_control(motor, 512); // middle value
    motor->servo_pos_deg = 512;
    motor->servo_status = SERVO_INPOSITION_FLAG;
    motor->servo_error_status = SERVO_STATUS_OK;

}

//---------------------- Useful functions -----------------

void HERKULEX_servo_set_torque(SmartServo *motor, uint8_t cmdTorque) 
{
    uint8_t packet_size = SERVO_MIN_PACKET_SIZE + 3;
    uint8_t txBuf[SERVO_MIN_PACKET_SIZE + 3];
    
    txBuf[0] = SERVO_HEADER;                // Packet Header (0xFF)
    txBuf[1] = SERVO_HEADER;                // Packet Header (0xFF)
    txBuf[2] = packet_size;                 // Packet Size
    txBuf[3] = motor->servo_id;             // Servo ID
    txBuf[4] = SERVO_CMD_RAM_WRITE;         // Command Ram Write (0x03)

    txBuf[7] = SERVO_RAM_TORQUE_CONTROL;    // Address 52
    txBuf[8] = SERVO_BYTE1;                 // Length
    txBuf[9] = cmdTorque;                   // Torque command

    // Checksum1 = (PacketSize ^ pID ^ CMD ^ Data[0] ^ Data[1] ^ ... ^ Data[n]) & 0xFE
    // Checksum2 = (~Checksum1)&0xFE
    txBuf[5] = (txBuf[2]^txBuf[3]^txBuf[4]^txBuf[7]^txBuf[8]^txBuf[9]) & 0xFE;
    txBuf[6] = (~txBuf[5])&0xFE;

    uart_write_buff(motor->servo_uart, txBuf, packet_size);
}

// move servo to specific position, between 0 and 1023, 512 being middle
void HERKULEX_position_control(SmartServo *motor, uint16_t position, uint8_t playtime) 
{
    //if values are out of bounds return
    if (position > 1023) return;
    if (playtime > 255) return;

    uint8_t goalposition_msb = position >> 8;
    uint8_t goalposition_lsb = position & 0xff;

    uint8_t packet_size = SERVO_MIN_PACKET_SIZE + 5;
    uint8_t txBuf[SERVO_MIN_PACKET_SIZE + 5];
    
    txBuf[0]  = SERVO_HEADER;           // Packet Header (0xFF)
    txBuf[1]  = SERVO_HEADER;           // Packet Header (0xFF)
    txBuf[2]  = packet_size;            // Packet Size
    txBuf[3]  = motor->servo_id;               
    txBuf[4]  = SERVO_CMD_I_JOG;        // Command I JOG (0x06)

    txBuf[7]  = goalposition_lsb;              
    txBuf[8]  = goalposition_msb;  
    txBuf[9]  = 4;                      // Position control Mode and green LED on
    txBuf[10] = motor->servo_id;        // Servo ID
    txBuf[11] = playtime;               // Time to goal
    
    // Checksum1 = (PacketSize ^ pID ^ CMD ^ Data[0] ^ Data[1] ^ ... ^ Data[n]) & 0xFE
    // Checksum2 = (~Checksum1)&0xFE
    txBuf[5] = (txBuf[2]^txBuf[3]^txBuf[4]^txBuf[7]^txBuf[8]^txBuf[9]^txBuf[10]^txBuf[11]) & 0xFE;
    txBuf[6] = (~txBuf[5])&0xFE;

    uart_write_buff(motor->servo_uart, txBuf, packet_size);
}

// Returns actual servo position to the motor struct, returns 1 for success and 0 for error
void HERKULEX_position_feedback(SmartServo *motor) 
{
    uint16_t position = 0;
    //read the position from servo
    position = HERKULEX_read_ram(motor, SERVO_RAM_CALIBRATED_POSITION, SERVO_BYTE2);

    //in the example the highest byte is AND with 0b00000011, this is because that is the max it could be.
    position = position & 0x03FF; // taken from the examples, for position they had  & 0x03 for the highest byte

    //check there was no errors
    if (motor->servo_error_status == SERVO_STATUS_OK && position != 0){
        motor->servo_pos_deg = position; // do we want to convert this to an angle? angle = position * 0.325
    }
    
}

//reboot the servo, clears RAM and reloads from ROM
void HERKULEX_reboot(SmartServo *motor){
    uint8_t packet_size = SERVO_MIN_ACK_PACKET_SIZE; //packet size for transmit
    uint8_t txBuf[SERVO_MIN_ACK_PACKET_SIZE];
    
    txBuf[0] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[1] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[2] = packet_size;                     // Packet Size
    txBuf[3] = motor->servo_id;                 // Servo ID
    txBuf[4] = SERVO_CMD_REBOOT;                // Reboot Command
    // Check Sum1 and Check Sum2
    txBuf[5] = (txBuf[2]^txBuf[3]^txBuf[4]) & SERVO_CHKSUM_MASK;
    txBuf[6] = (~txBuf[5])&SERVO_CHKSUM_MASK;
    
    // send packet
    uart_write_buff(motor->servo_uart, txBuf, packet_size);
}

// roll back ROM to default values, except IDs and Baudrates
void HERKULEX_rollback(SmartServo *motor){
    uint8_t packet_size = SERVO_MIN_ACK_PACKET_SIZE + 2; //packet size for transmit
    uint8_t txBuf[SERVO_MIN_ACK_PACKET_SIZE + 2];
    
    txBuf[0] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[1] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[2] = packet_size;                     // Packet Size
    txBuf[3] = motor->servo_id;                 // Servo ID
    txBuf[4] = SERVO_CMD_ROLLBACK;              // Rollback Command
    txBuf[7] = 1;                               // Skips ID rollback
    txBuf[8] = 1;                               // Skips Baudrate rollback

    // Check Sum1 and Check Sum2
    txBuf[5] = (txBuf[2]^txBuf[3]^txBuf[4]^txBuf[7]^txBuf[8]) & SERVO_CHKSUM_MASK;
    txBuf[6] = (~txBuf[5])&SERVO_CHKSUM_MASK;
    
    // send packet
    uart_write_buff(motor->servo_uart, txBuf, packet_size);
}



//------- ROM functions (maintained after reboot) ---------
//function to change a rom value
void HERKULEX_set_rom(SmartServo *motor, uint16_t reg, uint16_t value)
{

}
//function to read a rom value
uint16_t HERKULEX_read_rom(SmartServo *motor, uint16_t reg, uint8_t length)
{
    HERKULEX_read(motor, SERVO_CMD_ROM_READ, reg, length);
}

//------- RAM functions (lost after reboot) ------------
//function to change a ram value
void HERKULEX_set_ram(SmartServo *motor, uint16_t reg, uint16_t value)
{

}
//function to read a ram value
uint16_t HERKULEX_read_ram(SmartServo *motor, uint16_t reg, uint8_t length)
{
    HERKULEX_read(motor, SERVO_CMD_RAM_READ, reg, length);
}


//private read function
uint16_t HERKULEX_read(SmartServo *motor, uint8_t cmd, uint16_t reg, uint8_t length)
{
    //check length is a valid input (1 or 2)
    if (length != SERVO_BYTE1 && length != SERVO_BYTE2){
        return 0;
    }

    uint8_t packet_size = SERVO_MIN_ACK_PACKET_SIZE + 2; //packet size for transmit
    uint8_t txBuf[SERVO_MIN_ACK_PACKET_SIZE + 2];
    
    txBuf[0] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[1] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[2] = packet_size;                     // Packet Size
    txBuf[3] = motor->servo_id;                 // Servo ID
    txBuf[4] = SERVO_CMD_RAM_READ;              // RAM Read Command
    txBuf[7] = reg;                             // Register Address given
    txBuf[8] = length;                          // is SERVO_BYTE1 or SERVO_BYTE2 for either 1 or 2 bytes of data to return

    // Check Sum1 and Check Sum2
    txBuf[5] = (txBuf[2]^txBuf[3]^txBuf[4]^txBuf[7]^txBuf[8]) & SERVO_CHKSUM_MASK;
    txBuf[6] = (~txBuf[5])&SERVO_CHKSUM_MASK;
    
    // send packet
    uart_write_buff(motor->servo_uart, txBuf, packet_size);

    //delay here
    delay_microseconds(10);

    //recieve ACK packet
    //packet size is 12 or 13
    packet_size = (SERVO_MIN_ACK_PACKET_SIZE) + length + 2; //ACK returns the same sent infomation followed by data requested, followed by status error, and status detail.
    uint8_t rxBuf[SERVO_MIN_ACK_PACKET_SIZE + 2 + SERVO_BYTE2]; //this can't dynamically change length so make it the longer of the two
    
    uint16_t result = 0;

    uint8_t wait_counter;
    uint8_t wait_limit = 250;
    for (int ii = 0; ii<=packet_size-1; ii++) {
        wait_counter = 0;
        while(!uart_read_ready(motor->servo_uart))
        {
            wait_counter++;
            delay_microseconds(4); // at 115,200 bps, each cycle should be 8us
            if (wait_counter >= wait_limit){
                motor->servo_error_status = SERVO_ERROR_INVALID_PACKET;
                return 0;
            }
        }
        rxBuf[ii] = uart_read_byte(motor->servo_uart);
    }
    
    //---------------------- error checking -------------------
    // Checksums:
    // Checksum1
    uint8_t chksum1 = (rxBuf[2]^rxBuf[3]^rxBuf[4]^rxBuf[7]^rxBuf[8]^rxBuf[9]^rxBuf[10]^rxBuf[11]^rxBuf[12]) & SERVO_CHKSUM_MASK;
    //check
    if (chksum1 != rxBuf[5])
    {
        motor->servo_error_status = SERVO_ERROR_INVALID_PACKET; //checksum error
        return 0;
    }
    
    // // Checksum2
    uint8_t chksum2 = (~rxBuf[5]&SERVO_CHKSUM_MASK);
    if (chksum2 != rxBuf[6])
    {
        motor->servo_error_status = SERVO_ERROR_INVALID_PACKET; //checksum error
        return 0;
    }

    //Status error & status details
    motor->servo_error_status = rxBuf[10 + length]; //if length is 2, error status is rxBuf[12], if 1 rxBuf[11]
    motor->servo_status = rxBuf[9 + length];        //if length is 2, status is rxBuf[11], if 1 rxBuf[10]

    //check information recieved matches data requested, id, cmd, address and length
    if (txBuf[3] != rxBuf[3] || txBuf[4] != rxBuf[4] || txBuf[7] != rxBuf[7] || txBuf[8] != rxBuf[8] ){
        motor->servo_error_status = SERVO_ERROR_INVALID_PACKET; //not the information requested
        return 0;
    }

    //------------------- Extract data ---------
    if (length == SERVO_BYTE2){
        //result = ((rxBuf[10]&0x03)<<8) | rxBuf[9]; // not sure why the &0x03 bit was there, its from the example get position code
        result = (rxBuf[10]<<8) | rxBuf[9];
    }else{
        result = rxBuf[9];
    }
    
    //return recieved bytes
    return result;
}
