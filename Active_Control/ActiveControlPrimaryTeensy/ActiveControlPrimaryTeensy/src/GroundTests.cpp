#include "GroundTests.hpp"

void GroundTests::deflectionTest(float deflection, Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{

    float deflections[NUM_SERVOS] = {deflection, deflection, deflection, deflection};
    float actualDeflections[NUM_SERVOS];
    servoInterface.setDeflections(deflections);
    delay(1000);
    servoInterface.getDeflections(actualDeflections);
    logger.logInfo("Deflections are (in rad): " +
                   String(deflections[0]) + ", " +
                   String(deflections[1]) + ", " +
                   String(deflections[2]) + ", " +
                   String(deflections[3]));
    if (servoInterface.checkDeflections(deflection))
    {
        logger.logInfo("Deflections are within specified limits.");
    }
    else
    {
        logger.logWarning("Deflections are not within specified limits.");
    }

    logger.logInfo("Waiting 10 seconds for visual check...");
    delay(10000);
    servoInterface.setZeroDeflections();
}

void GroundTests::maxDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    servoInterface.setMode(Servo::ServoMode::Enabled);
    logger.logInfo("Starting max deflection test...");
    deflectionTest(MAX_DEFLECTION_RAD, servoInterface, logger);
    logger.logInfo("Finished max deflection test.");
    servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
}

void GroundTests::maxNegativeDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    servoInterface.setMode(Servo::ServoMode::Enabled);
    logger.logInfo("Starting min deflection test...");
    deflectionTest(-MAX_DEFLECTION_RAD, servoInterface, logger);
    logger.logInfo("Finished min deflection test.");
    servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
}

void GroundTests::zeroDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
    logger.logInfo("Starting zero deflection test...");
    deflectionTest(0, servoInterface, logger);
    logger.logInfo("Finished zero deflection test.");
    servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
}

void GroundTests::rapidDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    servoInterface.setMode(Servo::ServoMode::Enabled);
    float maxDeflections[NUM_SERVOS] = {MAX_DEFLECTION_RAD, MAX_DEFLECTION_RAD, MAX_DEFLECTION_RAD, MAX_DEFLECTION_RAD};
    float minDeflections[NUM_SERVOS] = {-MAX_DEFLECTION_RAD, -MAX_DEFLECTION_RAD, -MAX_DEFLECTION_RAD, -MAX_DEFLECTION_RAD};
    logger.logInfo("Starting rapid deflection test...");
    const int numTests = 5;
    unsigned long testDelays[numTests] = {500, 400, 300, 200, 100};
    for (int i = 0; i < numTests; i++)
    {
        logger.logInfo(String(testDelays[i]) + "ms interval");
        delay(500);
        for (int j = 0; j < 10; j++)
        {
            servoInterface.setDeflections(maxDeflections);
            delay(testDelays[i]);
            servoInterface.setDeflections(minDeflections);
            delay(testDelays[i]);
        }
        servoInterface.setZeroDeflections();
        delay(1000);
    };
    logger.logInfo("Finished rapid deflection test.");
    servoInterface.setMode(Servo::ServoMode::ZeroDeflection);
}

void GroundTests::wdtTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    logger.logInfo("Starting watchdog timer test");
    logger.logInfo("Setting deflections to max");
    float deflections[NUM_SERVOS] = {MAX_DEFLECTION_RAD, MAX_DEFLECTION_RAD, MAX_DEFLECTION_RAD, MAX_DEFLECTION_RAD};
    servoInterface.setDeflections(deflections);
    WDT_T4<WDT3> wdt;
    WDT_timings_t wdtConfig;
    elapsedMillis elapsedTime;
    int countNum = 1;
    unsigned long maxTimeBeforeFailure = 4000;
    wdtConfig.timeout = 50;
    logger.logInfo("Starting watchdog timer");
    wdt.begin(wdtConfig);
    while (elapsedTime < maxTimeBeforeFailure)
    {
        if (countNum == 1)
        {
            countNum++;
            logger.logInfo("Stopping feed in 3...");
        }
        if ((countNum == 2) & (elapsedTime > 1000))
        {
            countNum++;
            logger.logInfo("2...");
        }
        if ((countNum == 3) & (elapsedTime > 2000))
        {
            countNum++;
            logger.logInfo("1...");
        }
        if ((countNum == 4) & (elapsedTime > 3000))
        {
            countNum++;
            logger.logInfo("STOPPING FEED");
        }
        if (countNum != 5)
        {
            wdt.feed();
        }
    }
    logger.logError("Watchdog timer failed!");
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

void GroundTests::imuTest(SensorInterface::SensorInterface &sensorInterface, Logger::Logger &logger)
{
    logger.logInfo("Starting imu test...");
    float gx, gy, gz;
    float ax, ay, az;
    sensorInterface.update(1);
    sensorInterface.getLastGyroReading(gx, gy, gz);
    sensorInterface.getLastAccelReading(ax, ay, az);
    logger.logInfo("Gyro (rads-1) and accelerometer (ms-2) readings:");
    logger.logInfo("gx: " + String(gx));
    logger.logInfo("gy: " + String(gy));
    logger.logInfo("gz: " + String(gz));
    logger.logInfo("ax: " + String(ax));
    logger.logInfo("ay: " + String(ay));
    logger.logInfo("az: " + String(az));
    sensorInterface.resetOrientation();
    logger.logInfo("Finished imu test.");
}

void GroundTests::bmpTest(SensorInterface::SensorInterface &sensorInterface, Logger::Logger &logger)
{
    logger.logInfo("Starting bmp test...");
    float altitude;
    sensorInterface.update(1);
    sensorInterface.getLastAltitudeReading(altitude);
    logger.logInfo("Altitude: " + String(altitude) + "m");
    sensorInterface.resetOrientation();
    logger.logInfo("Finished bmp test.");
}

void GroundTests::showPIDs(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    uint16_t Kps[NUM_SERVOS];
    uint16_t Kis[NUM_SERVOS];
    uint16_t Kds[NUM_SERVOS];
    logger.logInfo("Printing servo PIDs");
    servoInterface.getPIDs(Kps, Kis, Kds);
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        switch (i)
        {
        case 0:
            logger.logInfo("X1:");
            break;
        case 1:
            logger.logInfo("X2:");
            break;
        case 2:
            logger.logInfo("Y1:");
            break;
        case 3:
            logger.logInfo("Y2:");
            break;
        }

        logger.logInfo("P: " + String(Kps[i]));
        logger.logInfo("I: " + String(Kis[i]));
        logger.logInfo("D: " + String(Kds[i]));
    }
}
