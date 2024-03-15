#include "GroundTests.hpp"

namespace LaunchRodTests
{

    void zeroDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void maxDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void maxNegativeDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void rapidDeflectionTest(Servo::ServoInterface &servoInterface, Logger::Logger &logger);
    void sdTest(Logger::Logger &logger);
}
