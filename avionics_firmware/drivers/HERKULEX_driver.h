/*
**************************************************
Driver for Herkulex DRS-0101 servos
**************************************************
Author: Oliver Martin

Herkulex Servo Manual 
http://www.hovis.co.kr/guide/herkulexeng.pdf

ARDUINO LIBRARY FOUND AT:
https://robottini.altervista.org/dongbu-herkulex-arduino-library-2

Very helpful C library: (as main one is C++)
https://github.com/hamadbinghalib/HerkulexSmartServoCLibrary/tree/main 
*/

#ifndef HERKULEX_DRIVER_H
#define HERKULEX_DRIVER_H

#include "mcu.h"

//------------------------------------------------------------------------------
// Herkulex ROM Register
#define SERVO_ROM_MODEL_NO1                       0
#define SERVO_ROM_MODEL_NO2                       1
#define SERVO_ROM_VERSION1                        2
#define SERVO_ROM_VERSION2                        3
#define SERVO_ROM_BAUD_RATE                       4
#define SERVO_ROM_RESERVED5                       5
#define SERVO_ROM_ID                              6
#define SERVO_ROM_ACK_POLICY                      7
#define SERVO_ROM_ALARM_LED_POLICY                8
#define SERVO_ROM_TORQUE_POLICY                   9
#define SERVO_ROM_RESERVED10                      10
#define SERVO_ROM_MAX_TEMPERATURE                 11
#define SERVO_ROM_MIN_VOLTAGE                     12
#define SERVO_ROM_MAX_VOLTAGE                     13
#define SERVO_ROM_ACCELERATION_RATIO              14
#define SERVO_ROM_MAX_ACCELERATION_TIME           15
#define SERVO_ROM_DEAD_ZONE                       16
#define SERVO_ROM_SATURATOR_OFFSET                17
#define SERVO_ROM_SATURATOR_SLOPE                 (18 << 8) // 18 & 19  // 2Byte
#define SERVO_ROM_PWM_OFFSET                      20
#define SERVO_ROM_MIN_PWM                         21
#define SERVO_ROM_MAX_PWM                         22  // 2Byte
#define SERVO_ROM_OVERLOAD_PWM_THRESHOLD          24  // 2Byte
#define SERVO_ROM_MIN_POSITION                    26  // 2Byte
#define SERVO_ROM_MAX_POSITION                    28  // 2Byte
#define SERVO_ROM_POSITION_KP                     30  // 2Byte
#define SERVO_ROM_POSITION_KD                     32  // 2Byte
#define SERVO_ROM_POSITION_KI                     34  // 2Byte
#define SERVO_ROM_POSITION_FEEDFORWARD_1ST_GAIN   36  // 2Byte
#define SERVO_ROM_POSITION_FEEDFORWARD_2ND_GAIN   38  // 2Byte
#define SERVO_ROM_RESERVED40                      40  // 2Byte
#define SERVO_ROM_RESERVED42                      42  // 2Byte
#define SERVO_ROM_LED_BLINK_PERIOD                44
#define SERVO_ROM_ADC_FAULT_CHECK_PERIOD          45
#define SERVO_ROM_PACKET_GARBAGE_CHECK_PERIOD     46
#define SERVO_ROM_STOP_DETECTION_PERIOD           47
#define SERVO_ROM_OVERLOAD_DETECTION_PERIOD       48   
#define SERVO_ROM_STOP_THRESHOLD                  49
#define SERVO_ROM_INPOSITION_MARGIN               50
#define SERVO_ROM_RESERVED51                      51
#define SERVO_ROM_RESERVED52                      52
#define SERVO_ROM_CALIBRATION_DIFFERENCE          53

//------------------------------------------------------------------------------
// Herkulex RAM Register
#define SERVO_RAM_ID                              0
#define SERVO_RAM_ACK_POLICY                      1
#define SERVO_RAM_ALARM_LED_POLICY                2
#define SERVO_RAM_TORQUE_POLICY                   3
#define SERVO_RAM_RESERVED4                       4
#define SERVO_RAM_MAX_TEMPERATURE                 5
#define SERVO_RAM_MIN_VOLTAGE                     6
#define SERVO_RAM_MAX_VOLTAGE                     7
#define SERVO_RAM_ACCELERATION_RATIO              8
#define SERVO_RAM_MAX_ACCELERATION                9
#define SERVO_RAM_DEAD_ZONE                       10
#define SERVO_RAM_SATURATOR_OFFSET                11
#define SERVO_RAM_SATURATOR_SLOPE                 12 // 2Byte
#define SERVO_RAM_PWM_OFFSET                      14
#define SERVO_RAM_MIN_PWM                         15
#define SERVO_RAM_MAX_PWM                         16 // 2Byte
#define SERVO_RAM_OVERLOAD_PWM_THRESHOLD          18 // 2Byte
#define SERVO_RAM_MIN_POSITION                    20 // 2Byte
#define SERVO_RAM_MAX_POSITION                    22 // 2Byte
#define SERVO_RAM_POSITION_KP                     24 // 2Byte
#define SERVO_RAM_POSITION_KD                     26 // 2Byte
#define SERVO_RAM_POSITION_KI                     28 // 2Byte
#define SERVO_RAM_POSITION_FEEDFORWARD_1ST_GAIN   30 // 2Byte
#define SERVO_RAM_POSITION_FEEDFORWARD_2ND_GAIN   32 // 2Byte
#define SERVO_RAM_RESERVED34                      34 // 2Byte
#define SERVO_RAM_RESERVED36                      36 // 2Byte
#define SERVO_RAM_LED_BLINK_PERIOD                38
#define SERVO_RAM_ADC_FAULT_DETECTION_PERIOD      39
#define SERVO_RAM_PACKET_GARBAGE_DETECTION_PERIOD 40
#define SERVO_RAM_STOP_DETECTION_PERIOD           41
#define SERVO_RAM_OVERLOAD_DETECTION_PERIOD       42
#define SERVO_RAM_STOP_THRESHOLD                  43
#define SERVO_RAM_INPOSITION_MARGIN               44
#define SERVO_RAM_RESERVED45                      45
#define SERVO_RAM_RESERVED46                      46
#define SERVO_RAM_CALIBRATION_DIFFERENCE          47
#define SERVO_RAM_STATUS_ERROR                    48
#define SERVO_RAM_STATUS_DETAIL                   49
#define SERVO_RAM_RESERVED50                      50
#define SERVO_RAM_RESERVED51                      51 
#define SERVO_RAM_TORQUE_CONTROL                  52
#define SERVO_RAM_LED_CONTROL                     53
#define SERVO_RAM_VOLTAGE                         54
#define SERVO_RAM_TEMPERATURE                     55
#define SERVO_RAM_CURRENT_CONTROL_MODE            56
#define SERVO_RAM_TICK                            57
#define SERVO_RAM_CALIBRATED_POSITION             58 // 2Byte
#define SERVO_RAM_ABSOLUTE_POSITION               60 // 2Byte
#define SERVO_RAM_DIFFERENTIAL_POSITION           62 // 2Byte
#define SERVO_RAM_PWM                             64 // 2Byte
#define SERVO_RAM_RESERVED66                      66 // 2Byte
#define SERVO_RAM_ABSOLUTE_GOAL_POSITION          68 // 2Byte
#define SERVO_RAM_ABSOLUTE_DESIRED_TRAJECTORY_POSITION    70 // 2Byte
#define SERVO_RAM_DESIRED_VELOCITY                72 // 2Byte

//------------------------------------------------------------------------------
// Request Packet [To Servo Module] 
#define SERVO_CMD_ROM_WRITE  0x01    // Write Length number of values to EEP Register Address
#define SERVO_CMD_ROM_READ   0x02    // Request Length number of values from EEP Register Address
#define SERVO_CMD_RAM_WRITE  0x03    // Write Length number of values to RAM Register Address
#define SERVO_CMD_RAM_READ   0x04    // Request Lenght number of values from RAM Register Address
#define SERVO_CMD_I_JOG      0x05    // Able to send JOG command to maximum 43 servos (operate timing of individual Servo)
#define SERVO_CMD_S_JOG      0x06    // Able to send JOG command to maximum 53 servos (operate simultaneously at same time)
#define SERVO_CMD_STAT       0x07    // Status Error, Status Detail request
#define SERVO_CMD_ROLLBACK   0x08    // Change all EEP Regsters to Factory Default value
#define SERVO_CMD_REBOOT     0x09    // Request Reboot

//------------------------------------------------------------------------------
// ACK Packet [To Controller(ACK)]
#define SERVO_CMD_ACK_MASK   0x40 // ACK Packet CMD is Request Packet CMD + 0x40
#define SERVO_CMD_EEP_WRITE_ACK   (SERVO_CMD_ROM_WRITE|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_EEP_READ_ACK    (SERVO_CMD_ROM_READ|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_RAM_WRITE_ACK   (SERVO_CMD_RAM_WRITE|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_RAM_READ_ACK    (SERVO_CMD_RAM_READ|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_I_JOG_ACK       (SERVO_CMD_I_JOG|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_S_JOG_ACK       (SERVO_CMD_S_JOG|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_STAT_ACK        (SERVO_CMD_STAT|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_ROLLBACK_ACK    (SERVO_CMD_ROLLBACK|SERVO_CMD_ACK_MASK)
#define SERVO_CMD_REBOOT_ACK      (SERVO_CMD_REBOOT|SERVO_CMD_ACK_MASK)

//------------------------------------------------------------------------------
// Status Error
#define SERVO_STATUS_OK                        0x00
#define SERVO_ERROR_EXCEED_INPUT_VOLTAGE       0x01
#define SERVO_ERROR_EXCEED_POT_LIMIT           0x02
#define SERVO_ERROR_EXCEED_TEMPERATURE_LIMIT   0x04
#define SERVO_ERROR_INVALID_PACKET             0x08
#define SERVO_ERROR_OVERLOAD                   0x10
#define SERVO_ERROR_DRIVER_FAULT               0x20
#define SERVO_ERROR_EEP_REG_DISTORT            0x40

//------------------------------------------------------------------------------
// Status Detail
#define SERVO_MOVING_FLAG                      0x01
#define SERVO_INPOSITION_FLAG                  0x02
#define SERVO_CHECKSUM_ERROR                   0x04 // Invalid packet`s detailed information
#define SERVO_UNKNOWN_COMMAND                  0x08 // Invalid packet`s detailed information
#define SERVO_EXCEED_REG_RANGE                 0x10 // Invalid packet`s detailed information
#define SERVO_GARBAGE_DETECTED                 0x20 // Invalid packet`s detailed information
#define SERVO_MOTOR_ON_FLAG                    0x40

//------------------------------------------------------------------------------
// Header
#define SERVO_HEADER                              0xFF

// Size
#define SERVO_MIN_PACKET_SIZE                     7
#define SERVO_MIN_ACK_PACKET_SIZE                 9
#define SERVO_WRITE_PACKET_SIZE                   13
#define SERVO_MAX_PACKET_SIZE                     223
#define SERVO_MAX_DATA_SIZE                       (SERVO_MAX_PACKET_SIZE-SERVO_MIN_PACKET_SIZE)

// ID
#define SERVO_MAX_PID                             0xFD
#define SERVO_DEFAULT_ID                          0xFD
#define SERVO_MAX_ID                              0xFD
#define SERVO_BROADCAST_ID                        0xFE

// Checksum
#define SERVO_CHKSUM_MASK                         0xFE

// Torque CMD
#define SERVO_TORQUE_FREE                         0x00
#define SERVO_BREAK_ON                            0x40
#define SERVO_TORQUE_ON                           0x60

// Register Size
#define SERVO_BYTE1                               1
#define SERVO_BYTE2                               2

// Jog Set CMD
#define SERVO_STOP                                0x01
#define SERVO_POS_MODE                            0x00
#define SERVO_TURN_MODE                           0x02
#define SERVO_LED_GREEN                           0x04
#define SERVO_LED_BLUE                            0x08
#define SERVO_LED_RED                             0x10

typedef struct SmartServo
{
    uint8_t servo_id;
    uint8_t servo_pos_deg;
    uint8_t servo_status;
    uint8_t servo_error_status;
    uart servo_uart;
} SmartServo;

#pragma region Public

void HERKULEX_init(SmartServo *motor);

void HERKULEX_servo_set_torque(SmartServo *motor, uint8_t cmdTorque);

void HERKULEX_position_control(SmartServo *motor, uint16_t position, uint8_t playtime);

void HERKULEX_position_feedback(SmartServo *motor);

//***reboot function
//***factory reset function (called rollback)



//------------------ ROM functions (maintained after reboot) ---------

//function to change a rom value
void HERKULEX_set_rom(SmartServo *motor, uint16_t reg, uint16_t value);

//function to read a rom value
//reg is a defined register to read, length is either SERVO_BYTE1 or SERVO_BYTE2
uint16_t HERKULEX_read_rom(SmartServo *motor, uint16_t reg, uint8_t length);

//----------------- RAM functions (lost after reboot) ------------
//function to change a ram value
void HERKULEX_set_ram(SmartServo *motor, uint16_t reg, uint16_t value);

//function to read a ram value
//reg is a defined register to read, length is either SERVO_BYTE1 or SERVO_BYTE2
uint16_t HERKULEX_read_ram(SmartServo *motor, uint16_t reg, uint8_t length);

#pragma endregion Public
#pragma region Private
//------------------ function with building blocks for ram and rom read/writes -------------------------

uint16_t HERKULEX_read(SmartServo *motor, uint8_t cmd, uint16_t reg, uint8_t length);

#pragma endregion Private

#endif