
#include "HERKULEX_driver.h"

void HERKULEX_init(SmartServo *motor)
{
    //Any Uart initialisation needed?

    //starting sequence
    HERKULEX_servo_set_torque(motor, SERVO_TORQUE_ON);
    HERKULEX_position_control(motor, 512);
}

//---------------------- Useful functions -----------------

void HERKULEX_servo_set_torque(SmartServo *motor, uint8_t cmdTorque) 
{
    
    uint8_t packet_size = 10;
    uint8_t txBuf[packet_size];
    
    txBuf[0] = SERVO_HEADER;              // Packet Header (0xFF)
    txBuf[1] = SERVO_HEADER;              // Packet Header (0xFF)
    txBuf[2] = SERVO_MIN_PACKET_SIZE + 3; // Packet Size
    txBuf[3] = motor->servo_id;                  // Servo ID
    txBuf[4] = SERVO_CMD_RAM_WRITE;       // Command Ram Write (0x03)
    txBuf[5] = 0;                   // Checksum1
    txBuf[6] = 0;                   // Checksum2
    txBuf[7] = SERVO_RAM_TORQUE_CONTROL;  // Address 52
    txBuf[8] = SERVO_BYTE1;               // Length
    txBuf[9] = cmdTorque;            // Torque ON

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

    uint8_t packet_size = 12;

    uint8_t txBuf[packet_size];
    
    txBuf[0]  = SERVO_HEADER;                 // Packet Header (0xFF)
    txBuf[1]  = SERVO_HEADER;                 // Packet Header (0xFF)
    txBuf[2]  = SERVO_MIN_PACKET_SIZE + 5;    // Packet Size
    txBuf[3]  = motor->servo_id;               
    txBuf[4]  = SERVO_CMD_I_JOG;              // Command I JOG (0x06)
    txBuf[5]  = 0;                      // Checksum1
    txBuf[6]  = 0;                      // Checksum2
    txBuf[7]  = goalposition_lsb;              
    txBuf[8]  = goalposition_msb;  
    txBuf[9]  = 4;
    txBuf[10] = motor->servo_id;      // Pos Mode and LED on/off
    txBuf[11] = playtime;                     // Servo ID
    
    // Checksum1 = (PacketSize ^ pID ^ CMD ^ Data[0] ^ Data[1] ^ ... ^ Data[n]) & 0xFE
    // Checksum2 = (~Checksum1)&0xFE
    txBuf[5] = (txBuf[2]^txBuf[3]^txBuf[4]^txBuf[7]^txBuf[8]^txBuf[9]^txBuf[10]^txBuf[11]) & 0xFE;
    txBuf[6] = (~txBuf[5])&0xFE;

    uart_write_buff(motor->servo_uart, txBuf, packet_size);
}

// Returns actual servo position to the motor struct, returns 1 for success and 0 for error
uint8_t HERKULEX_position_feedback(SmartServo *motor) 
{
    uint8_t position = 0;

    uint8_t packet_size = 9;
    uint8_t txBuf[packet_size];
    
    txBuf[0] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[1] = SERVO_HEADER;                    // Packet Header (0xFF)
    txBuf[2] = SERVO_MIN_PACKET_SIZE + 2;       // Packet Size
    txBuf[3] = motor->servo_id;                 // Servo ID
    txBuf[4] = SERVO_CMD_RAM_READ;              // Status Error, Status Detail request
    txBuf[5] = 0;                               // Checksum1
    txBuf[6] = 0;                               // Checksum2    
    txBuf[7] = SERVO_RAM_CALIBRATED_POSITION;   // Address 52
    txBuf[8] = SERVO_BYTE2;                     // Address 52 and 53      

    // Check Sum1 and Check Sum2
    txBuf[5] = (txBuf[2]^txBuf[3]^txBuf[4]^txBuf[7]^txBuf[8]) & 0xFE;
    txBuf[6] = (~txBuf[5])&0xFE;

    // send packet
    uart_write_buff(motor->servo_uart, txBuf, packet_size);
    
    //maybe add a delay here
    delay_microseconds(10);

    //receive packet
    packet_size = 13
    uint8_t rxBuf[packet_size];
    uint8_t wait_counter;
    uint8_t wait_limit = 250;
    for (int ii = 0; ii<=packet_size-1; ii++) {
        wait_counter = 0;
        while(!uart_read_ready(motor->servo_uart))
        {
            wait_counter++;
            delay_microseconds(4); // at 115,200 bps, each cycle should be 8us
            if (wait_counter >= wait_limit){
                return 0; //error, took too long
            }
        }
        motor->servo_rx_packet[ii] = uart_read_byte(motor->servo_uart);
    }

    // Checksum1
    uint8_t chksum1 = (rxBuf[2]^rxBuf[3]^rxBuf[4]^rxBuf[7]^rxBuf[8]^rxBuf[9]^rxBuf[10]^rxBuf[11]^rxBuf[12]) & 0xFE;    
    //check
    if (chksum1 != rxBuf[5])
    {
        return 0; //checksum error
    }
    
    // // Checksum2
    uint8_t chksum2 = (~rxBuf[5]&0xFE);
    if (chksum2 != rxBuf[6])
    {
        return 0; //checksum error
    }

    position = ((rxBuf[10]&0x03)<<8) | rxBuf[9];
    
    motor->servo_pos_deg = position;
    return 1;
}

