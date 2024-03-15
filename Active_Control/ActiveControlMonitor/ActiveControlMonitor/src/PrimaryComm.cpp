#include "PrimaryComm.hpp"

void PrimaryComm::PrimaryComm::start()
{
    pinMode(LAUNCH_SIG_PIN, INPUT);
    pinMode(FAILURE_SIG_PIN, OUTPUT);
    pinMode(WATCHDOG_SIG_PIN, OUTPUT);
    digitalWrite(FAILURE_SIG_PIN, LOW);
    digitalWrite(WATCHDOG_SIG_PIN, LOW);
}

bool PrimaryComm::PrimaryComm::readLaunchSignal()
{
    return digitalRead(LAUNCH_SIG_PIN);
}

void PrimaryComm::PrimaryComm::startNoFailureSignal()
{
    digitalWrite(FAILURE_SIG_PIN, HIGH);
}

void PrimaryComm::PrimaryComm::startWatchdogSignal()
{
    digitalWrite(WATCHDOG_SIG_PIN, HIGH);
}

void PrimaryComm::PrimaryComm::endNoFailureSignal()
{
    digitalWrite(FAILURE_SIG_PIN, LOW);
}

void PrimaryComm::PrimaryComm::endWatchdogSignal()
{
    digitalWrite(WATCHDOG_SIG_PIN, LOW);
}
