#include "GroundTests.hpp"

void GroundTests::imuTest(SensorInterface::SensorInterface &sensorInterface, Logger::Logger &logger)
{
    logger.logInfo("Starting imu test...");
    float gx, gy, gz;
    float ax, ay, az;
    bool gyroComplete = false;
    bool accelComplete = false;
    while ((gyroComplete == false) | (accelComplete == false))
    {
        sensorInterface.update(1);
        if ((sensorInterface.wasOrientationUpdated()) & (gyroComplete == false))
        {
            sensorInterface.getLastGyroReading(gx, gy, gz);
            gyroComplete = true;
        }
        if ((sensorInterface.wasAccelUpdated()) & (accelComplete == false))
        {
            sensorInterface.getLastAccelReading(ax, ay, az);
            accelComplete = true;
        }
    }
    logger.logInfo("Gyro (degs-1) and accelerometer (g) readings:");
    logger.logInfo("gx: " + String(gx, 5));
    logger.logInfo("gy: " + String(gy, 5));
    logger.logInfo("gz: " + String(gz, 5));
    logger.logInfo("ax: " + String(ax, 5));
    logger.logInfo("ay: " + String(ay, 5));
    logger.logInfo("az: " + String(az, 5));
    sensorInterface.resetOrientation();
    logger.logInfo("Finished imu test.");
}

void GroundTests::sdTest(Logger::Logger &logger)
{
    logger.logInfo("Starting sd test...");
    DataPacket packet{};
    packet.message = OutputMessageCode::SDTest;
    logger.logData(packet);
    switch (logger.error)
    {
    case Logger::LoggerError::SDWriteError:
        logger.logError("Could not write to sd!");
        break;
    case Logger::LoggerError::None:
        logger.logInfo("Successfully wrote to SD.");
        break;
    default:
        logger.logError("Unknown SD error!");
        break;
    }
}