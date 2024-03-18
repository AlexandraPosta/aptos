#ifndef SERVO_INTERFACE_H
#define SERVO_INTERFACE_H

#include "Arduino.h"
#include "HerkulexServo.h"
#include "ValueDefinitions.hpp"

#define SERVO_ID_X1 1
#define SERVO_ID_X2 2
#define SERVO_ID_Y1 3
#define SERVO_ID_Y2 4

#define SERVO_BAUD 115200

namespace Servo
{
    enum class ServoInterfaceError
    {
        None
    };
    enum class ServoMode
    {
        Enabled,
        Disabled,
        ZeroDeflection,
    };
    class ServoInterface
    {
    private:
        HardwareSerial &servoSerial = Serial3;
        HerkulexServoBus servoBus{servoSerial};
        HerkulexServo servoX1{servoBus, SERVO_ID_X1};
        HerkulexServo servoX2{servoBus, SERVO_ID_X2};
        HerkulexServo servoY1{servoBus, SERVO_ID_Y1};
        HerkulexServo servoY2{servoBus, SERVO_ID_Y2};
        HerkulexServo servos[NUM_SERVOS] = {servoX1, servoX2, servoY1, servoY2};
        int16_t zeroOffset[NUM_SERVOS] = {0, 0, 0, 0};

        static uint16_t radsToInt(float rads)
        {
            return 512 + roundf(rads * 176.294706194);
        }
        static float intToRads(int value)
        {
            return (value - 512) * 0.00567232006;
        }
        ServoMode currentMode = ServoMode::ZeroDeflection;

    public:
        ServoInterface(){};
        ServoInterfaceError error = ServoInterfaceError::None;
        void quickStartAndDeflect();
        void start();
        void setDeflections(float deflections[NUM_SERVOS]);
        void setZeroDeflections();
        void setTorquesOn();
        void setTorquesOff();
        void setMode(ServoMode mode);
        void getDeflections(float actualDeflections[NUM_SERVOS]);
        bool checkZeroDeflection();
        bool checkDeflections(float deflection);
        void setPID(uint16_t Kp, uint16_t Ki, uint16_t Kd);
        void setPIDSingle(uint8_t servoNum, uint16_t Kp, uint16_t Ki, uint16_t Kd);
        void getPIDs(uint16_t Kps[NUM_SERVOS], uint16_t Kis[NUM_SERVOS], uint16_t Kds[NUM_SERVOS]);
        void getPID(uint8_t servoNum, uint16_t &Kp, uint16_t &Ki, uint16_t &Kd);
        void setPIDEEP(uint16_t Kp, uint16_t Ki, uint16_t Kd);
        void setPIDEEPSingle(uint8_t servoNum, uint16_t Kp, uint16_t Ki, uint16_t Kd);
        void resetPID();
        int16_t addZeroOffset(uint8_t servoNum);
        int16_t subZeroOffset(uint8_t servoNum);
    };
}
#endif