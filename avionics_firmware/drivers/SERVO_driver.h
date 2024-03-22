#ifndef SERVO_DRIVER_HRegisters
#define SERVO_DRIVER_H

// DATASHEET: https://files.waveshare.com/upload/2/27/Communication_Protocol_User_Manual-EN%28191218-0923%29.pdf
// Memory map: https://files.waveshare.com/upload/2/27/sts3215_%20memory_table.xlsx 

//================== INCLUDES ===================
#include "mcu.h"

//------------------------------------------------------------------------------
// Registers
#define SERVO_REG_FIRMWARE_MAJOR            0x00
#define SERVO_REG_FIRMWARE_MINOR            0x01
#define SERVO_REG_SERVO_MAJOR               0x03
#define SERVO_REG_SERVO_MINOR               0x04
#define SERVO_REG_ID                        0x05
#define SERVO_REG_BAUDRATE                  0x06
#define SERVO_REG_RESPONSE_DELAY            0x07
#define SERVO_REG_RESPONSE_STATUS_LEVEL     0x08
#define SERVO_REG_MINIMUM_ANGLE             0x09
#define SERVO_REG_MAXIMUM_ANGLE             0x0B
#define SERVO_REG_MAXIMUM_TEMPERATURE       0x0D
#define SERVO_REG_MAXIMUM_VOLTAGE           0x0E
#define SERVO_REG_MINIMUM_VOLTAGE           0x0F
#define SERVO_REG_MAXIMUM_TORQUE            0x10
#define SERVO_REG_UNLOADING_CONDITION       0x13
#define SERVO_REG_LED_ALARM_CONDITION       0x14
#define SERVO_REG_POS_PROPORTIONAL_GAIN     0x15
#define SERVO_REG_POS_DERIVATIVE_GAIN       0x16
#define SERVO_REG_POS_INTEGRAL_GAIN         0x17
#define SERVO_REG_MINIMUM_STARTUP_FORCE     0x18
#define SERVO_REG_CK_INSENSITIVE_AREA       0x1A
#define SERVO_REG_CCK_INSENSITIVE_AREA      0x1B
#define SERVO_REG_CURRENT_PROTECTION_TH     0x1C
#define SERVO_REG_ANGULAR_RESOLUTION        0x1E
#define SERVO_REG_POSITION_CORRECTION       0x1F
#define SERVO_REG_OPERATION_MODE            0x21
#define SERVO_REG_TORQUE_PROTECTION_TH      0x22
#define SERVO_REG_TORQUE_PROTECTION_TIME    0x23
#define SERVO_REG_OVERLOAD_TORQUE           0x24
#define SERVO_REG_SPEED_PROPORTIONAL_GAIN   0x25
#define SERVO_REG_OVERCURRENT_TIME          0x26
#define SERVO_REG_SPEED_INTEGRAL_GAIN       0x27
#define SERVO_REG_TORQUE_SWITCH             0x28
#define SERVO_REG_TARGET_ACCELERATION       0x29
#define SERVO_REG_TARGET_POSITION           0x2A
#define SERVO_REG_RUNNING_TIME              0x2C
#define SERVO_REG_RUNNING_SPEED             0x2E
#define SERVO_REG_TORQUE_LIMIT              0x30
#define SERVO_REG_WRITE_LOCK                0x37
#define SERVO_REG_CURRENT_POSITION          0x38
#define SERVO_REG_CURRENT_SPEED             0x3A
#define SERVO_REG_CURRENT_DRIVE_VOLTAGE     0x3C
#define SERVO_REG_CURRENT_VOLTAGE           0x3E
#define SERVO_REG_CURRENT_TEMPERATURE       0x3F
#define SERVO_REG_ASYNCHRONOUS_WRITE_ST     0x40
#define SERVO_REG_STATUS                    0x41
#define SERVO_REG_MOVING_STATUS             0x42
#define SERVO_REG_CURRENT_CURRENT           0x45


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
#define SERVO_MAX_WRITE_SIZE                      8 //this can be increased
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
	@brief Initialises a servo uart to single wire
	@note 
*/
void ServoUartInit(USART_TypeDef* uart);

/**
	@brief enables or disables power to servos
	@note 
*/
void ServoEnable(bool enable);

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

/**
	@brief Changes the ID of a servo
	@note 
	@return 
*/
void ServoSetId(SmartServo* servo, uint8_t new_id);

/**
	@brief Sets target angle of the servo
	@note angle in milli degrees
*/
void ServoSetTargetAngle(SmartServo* servo, int32_t target_angle_mdeg);

/**
	@brief Sets target position of servo
	@note position is a value between 0-4096
*/
void ServoSetTargetPosition(SmartServo* servo, uint16_t target_postion);

/**
	@brief Sets pid values for the position control of the servo
	@note all values are 0-254, use 255 (0xFF) to skip assignement.
	@note Default pid values are 32, 32, 0
*/
void ServoSetPID(SmartServo* servo, uint8_t p, uint8_t i, uint8_t d);

/**
	@brief Gets current angle of servo
	@note
    @return angle in milli degrees
*/
int32_t ServoGetCurrentAngle(SmartServo* servo);

/**
	@brief Gets current position of servo
	@note
    @return position between 0-4096
*/
uint16_t ServoGetCurrentPosition(SmartServo* servo);


//========================== Private functions ==============================================
/**
	@brief Reads two registers to return a 16-bit result
	@note pass the lower number register
	@return 16 bit result of the 2 registers
*/
uint16_t ServoReadTwoBytes(SmartServo* servo, uint8_t address);

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