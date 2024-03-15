#include "MonitorComm.hpp"

void MonitorComm::MonitorComm::start()
{
    pinMode(LAUNCH_SIG_PIN, OUTPUT);
    pinMode(FAILURE_SIG_PIN, INPUT);
    pinMode(WATCHDOG_SIG_PIN, INPUT);
    digitalWrite(LAUNCH_SIG_PIN, LOW);
}

void MonitorComm::MonitorComm::startLaunchSignal()
{
    digitalWrite(LAUNCH_SIG_PIN, HIGH);
}

bool MonitorComm::MonitorComm::readFailureSignal()
{
    // use a not because failure is LOW
    return !digitalReadFast(FAILURE_SIG_PIN);
}

bool MonitorComm::MonitorComm::readWatchdogSignal()
{
    // use a not because failure is LOW
    return !digitalReadFast(WATCHDOG_SIG_PIN);
}
