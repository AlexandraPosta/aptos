#ifndef DATA_PACKET_H
#define DATA_PACKET_H
#include <Arduino.h>
#include "OutputMessageCodes.hpp"

struct DataPacket
{
    // timestamp
    unsigned long elapsedTime;

    // error message
    OutputMessageCode message = OutputMessageCode::None;

    // accel
    float ax, ay, az = 0.0f;

    // gyro
    float gx, gy, gz = 0.0f;

    // barometer
    float altitude = 0.0f;
    float verticalVelocity = 0.0f;

    // orientation
    float w, x, y, z = 0.0f;
    float angleToVertical = 0.0f;

    byte servoReturnData[30] = {0};

    String toString()
    {
        String msg = "";
        msg = msg + elapsedTime + ", " + message + "," + ax + "," + ay + "," + az + "," + gx + "," + gy + "," + gz + "," + altitude + "," + verticalVelocity + "," + w + "," + x + "," + y + "," + z + "," + angleToVertical;
        return msg;
    }
};

#endif