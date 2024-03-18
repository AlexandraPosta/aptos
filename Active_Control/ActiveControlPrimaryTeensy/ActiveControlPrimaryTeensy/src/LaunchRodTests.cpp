#include "LaunchRodTests.hpp"

void LaunchRodTests::zeroDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    GroundTests::zeroDeflectionTest(servoInterface, logger);
}

void LaunchRodTests::maxDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    GroundTests::maxDeflectionTest(servoInterface, logger);
}

void LaunchRodTests::maxNegativeDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    GroundTests::maxNegativeDeflectionTest(servoInterface, logger);
}

void LaunchRodTests::rapidDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger)
{
    GroundTests::rapidDeflectionTest(servoInterface, logger);
}

void LaunchRodTests::sdTest(Logger::Logger &logger)
{
    GroundTests::sdTest(logger);
}
