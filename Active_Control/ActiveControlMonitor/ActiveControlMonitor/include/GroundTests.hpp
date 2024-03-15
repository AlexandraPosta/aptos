#ifndef GROUND_TESTS_H
#define GROUND_TESTS_H

#include "BuzzerInterface.hpp"
#include "LEDInterface.hpp"
#include "SensorInterface.hpp"
#include "Logger.hpp"
#include "ValueDefinitions.hpp"

namespace GroundTests
{
    void sdTest(Logger::Logger &logger);
    void imuTest(SensorInterface::SensorInterface &sensorInterface, Logger::Logger &logger);
}
#endif