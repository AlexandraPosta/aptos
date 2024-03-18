#ifndef PRIMARY_COMM_H
#define PRIMARY_COMM_H
#include <Arduino.h>
#define LAUNCH_SIG_PIN 2
#define FAILURE_SIG_PIN 3
#define WATCHDOG_SIG_PIN 4

namespace PrimaryComm
{
    class PrimaryComm
    {
    public:
        void start();
        bool readLaunchSignal();
        void startNoFailureSignal();
        void startWatchdogSignal();
        void endNoFailureSignal();
        void endWatchdogSignal();
    };
}

#endif