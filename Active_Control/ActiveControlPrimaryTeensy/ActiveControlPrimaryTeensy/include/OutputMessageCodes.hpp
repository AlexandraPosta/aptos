
#ifndef OUTPUT_MESSAGE_CODES_H
#define OUTPUT_MESSAGE_CODES_H
#include <Arduino.h>

enum OutputMessageCode : uint8_t
{
    None = 0,
    UnknownError,

    // monitor failures
    RecievedFailureFromMonitor,
    RecievedWatchdogFromMonitor,

    // arming
    ArmFailed,
    ArmSuccess,
    LoggerError,

    // during launch
    GainUpdated,
    LaunchDetected,

    // primary failures
    PrimaryWatchdogNotFed,
    MaxOrientationReached,

    // state changes
    StateChangedToStartup,
    StateChangedToOffLaunchRodTest,
    StateChangedToOffLaunchRodCalibration,
    StateChangedToIdle,
    StateChangedToLaunchRodCalibration,
    StateChangedToLaunchRodTest,
    StateChangedToArm,
    StateChangedToArmed,
    StateChangedToBelowVmc,
    StateChangedToActiveControlEnabled,
    StateChangedToDescent,
    StateChangedToShutdown,

    // bluetooth errors
    BTStartUpError,
    BTConnectionError,

    // sensor errors
    IMUStartUpError,
    BarometerStartUpError,

    // tests
    SDTest,
};

#endif