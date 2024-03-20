#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

// DATASHEET: https://files.waveshare.com/upload/2/27/Communication_Protocol_User_Manual-EN%28191218-0923%29.pdf

//================== INCLUDES ===================
#include "mcu.h"

//------------------------------------------------------------------------------
// Instruction Types
#define SERVO_CMD_PING              0x01   // Query Working Status
#define SERVO_CMD_READ_DATA         0x02   // Query the character in the control table
#define SERVO_CMD_WRITE_DATA        0x03   // Write the character into the control table
#define SERVO_CMD_REGWRITE_DATA     0x04   // Similar to WRITE DATA, but the control character does not act immediately after writing until ACTION.
#define SERVO_CMD_ACTION            0x05   // Triggering the action of REG WRITE operation
#define SERVO_CMD_SYCNREAD_DATA     0x82   // Query multiple servos at the same time
#define SERVO_CMD_SYCNWRITE_DATA    0x83   // Controlling multiple servos at the same time.
#define SERVO_CMD_RESET             0x06   // Reset the control table to the factory value

//------------------------------------------------------------------------------
// Error types
#define SERVO_ERROR_INVALID_PACKET  0x01
#define SERVO_ERROR_CHECKSUM        0x02

//------------------------------------------------------------------------------
// Header
#define SERVO_HEADER                              0xFF

// Size
#define SERVO_MIN_PACKET_SIZE                     6
#define SERVO_MAX_WRITE_SIZE                       8 //this can be increased
#define SERVO_MAX_READ_SIZE                       8 //this can be increased

#define NUM_SERVOS          4

//============================== STRUCT ========================================
typedef struct SmartServo
{
    uint8_t servo_id;
    uint8_t servo_pos;
    uint8_t servo_pos_deg;
    uint8_t servo_status;
    uint8_t servo_error_status;
    USART_TypeDef* servo_uart;
} SmartServo;

//============================= FUNCTIONS ===================================================
/**
	@brief Initialises a servo
	@note sets up servo object, resets servo and moves to default position
	@return SmartServo struct
*/
SmartServo ServoInit(USART_TypeDef* uart, uint8_t id);

/**
	@brief sends reset cmd to servo
	@note 
	@return 
*/
void ServoReset(SmartServo* servo);

/**
	@brief Sends Ping cmd to servo
	@note 
	@return True if ping returned
*/
bool ServoPing(SmartServo* servo);

//================== Private functions =======================
/**
	@brief Writes data to servo
	@note 
*/
void ServoWriteData(SmartServo* servo, uint8_t address, uint8_t length, uint8_t* data);

/**
	@brief Reads data from servo
	@note Reads data starting at address and then the bytes after depending on length
	@return Results returned to result pointer
*/
void ServoReadData(SmartServo* servo, uint8_t address, uint8_t length, uint8_t* result);

#endif /* SERVO_DRIVER_H */