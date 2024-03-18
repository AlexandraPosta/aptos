#ifndef DATA_PACKET_H
#define DATA_PACKET_H
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
    float wRate, xRate, yRate, zRate = 0.0f;
    float roll, pitch, yaw = 0.0f;
    float rollRate, pitchRate, yawRate = 0.0f;

    // deflection
    float x1Target, x2Target, y1Target, y2Target = 0.0f;
    float x1Actual, x2Actual, y1Actual, y2Actual = 0.0f;

    // servo battery voltage
    float voltage = 0.0f;

    String toString()
    {
        String msg = "";
        msg = msg + elapsedTime + ", " + message + "," + ax + "," + ay + "," + az + "," + gx + "," + gy + "," + gz + "," + altitude + "," + verticalVelocity + "," + w + "," + x + "," + y + "," + z + "," + wRate + "," + xRate + "," + yRate + "," + zRate + "," + roll + "," + pitch + "," + yaw + "," + rollRate + "," + pitchRate + "," + yawRate + "," + x1Target + "," + x2Target + "," + y1Target + "," + y2Target + "," + x1Actual + "," + x2Actual + "," + y1Actual + "," + y2Actual;
        return msg;
    }
};

#endif