#ifndef MONITOR_COMM_H
#define MONITOR_COMM_H
#include <Arduino.h>
#define LAUNCH_SIG_PIN 25
#define FAILURE_SIG_PIN 26
#define WATCHDOG_SIG_PIN 27

namespace MonitorComm
{
    class MonitorComm
    {
    public:
        void start();
        void startLaunchSignal();
        bool readFailureSignal();
        bool readWatchdogSignal();
    };
}

#endif