#ifndef GROUND_TESTS_H
#define GROUND_TESTS_H

#include "BuzzerInterface.hpp"
#include "LEDInterface.hpp"
#include "ServoInterface.hpp"
#include "SensorInterface.hpp"
#include "Logger.hpp"
#include "ValueDefinitions.hpp"
#include "Watchdog_t4.h"

namespace GroundTests
{
    // canard tests
    void deflectionTest(float deflection, Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void zeroDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void maxDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void maxNegativeDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void rapidDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void wdtTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void sdTest(Logger::Logger &logger);
    void imuTest(SensorInterface::SensorInterface &sensorInterface, Logger::Logger &logger);
    void bmpTest(SensorInterface::SensorInterface &sensorInterface, Logger::Logger &logger);
    void showPIDs(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
}
#endif