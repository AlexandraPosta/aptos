
#ifndef OUTPUT_MESSAGE_CODES_H
#define OUTPUT_MESSAGE_CODES_H
#include <Arduino.h>

enum OutputMessageCode : uint8_t
{
    None = 0,
    UnknownError,

    // monitor failures
    SentFailureToPrimary,
    SentWatchdogToPrimary,

    // arming
    ArmFailed,
    ArmSuccess,
    LoggerError,

    // primary failures
    PrimaryWatchdogNotFed,

    LaunchDetected,

    // state changes
    StateChangedToStartup,
    StateChangedToOffLaunchRodTest,
    StateChangedToOffLaunchRodCalibration,
    StateChangedToIdle,
    StateChangedToLaunchRodCalibration,
    StateChangedToLaunchRodTest,
    StateChangedToArm,
    StateChangedToArmed,
    StateChangedToActiveControlEnabled,

    // bluetooth errors
    BTStartUpError,
    BTConnectionError,

    // sensor errors
    IMUStartUpError,
    BarometerStartUpError,

    // test
    SDTest,
};

#endif