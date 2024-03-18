#include "ReadVoltage.hpp"

void VoltageReader::start()
{
    pinMode(VOLTAGE_PIN, INPUT);
    analogReadAveraging(1);
}

void VoltageReader::setAverage(uint8_t numAverages)
{
    analogReadAveraging(numAverages);
}

void VoltageReader::readAndLog(Logger::Logger &logger)
{
    setAverage(20);
    logger.logInfo("Getting battery voltage at time " + String(millis()));
    float voltage;
    readVoltage(voltage);
    logger.logInfo("Battery voltage is: " + String(voltage));
    DataPacket packet{};
    packet.elapsedTime = millis();
    packet.voltage = voltage;
    logger.logData(packet);
    setAverage(1);
}

void VoltageReader::readVoltage(float &voltage)
{
    voltage = analogRead(VOLTAGE_PIN) * conversion;
}
