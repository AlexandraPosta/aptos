#ifndef READ_VOLTAGE_H
#define READ_VOLTAGE_H
#define VOLTAGE_PIN 40
#include <Arduino.h>
#include "Logger.hpp"

#define VOLTAGE_MAX 8.4
#define VOLTAGE_REDUCED_MAX 2.8

class VoltageReader
{
public:
    void readVoltage(float &voltage);
    void start();
    void setAverage(uint8_t numAverages);
    void readAndLog(Logger::Logger &logger);

private:
    const float conversion = (8.4 / 2.8) * (3.3 / 1023);
};

#endif