#ifndef PROFILER_H
#define PROFILER
#include <Arduino.h>
#define PROFILING 0
#if PROFILING

struct ProfilingTimers
{
    elapsedMillis gainUpdate;
    elapsedMillis orientationUpdate;
    elapsedMillis iteration;
};
ProfilingTimers profiler;
#endif
#define profileGainStart()           \
    do                               \
    {                                \
        if (PROFILING)               \
            profiler.gainUpdate = 0; \
    } while (0)
#define profileGainEnd()                                                            \
    do                                                                              \
    {                                                                               \
        if (PROFILING)                                                              \
            Serial.println("Time for gain update: " + String(profiler.gainUpdate)); \
    } while (0)
#define profileOrientationStart()           \
    do                                      \
    {                                       \
        if (PROFILING)                      \
            profiler.orientationUpdate = 0; \
    } while (0)
#define profileOrientationEnd()                                                                   \
    do                                                                                            \
    {                                                                                             \
        if (PROFILING)                                                                            \
            Serial.println("Time for orientation update: " + String(profiler.orientationUpdate)); \
    } while (0)

#define profileIterationStart()     \
    do                              \
    {                               \
        if (PROFILING)              \
            profiler.iteration = 0; \
    } while (0)
#define profileIterationEnd()                                                           \
    do                                                                                  \
    {                                                                                   \
        if (PROFILING)                                                                  \
            Serial.println("Time for iteration update: " + String(profiler.iteration)); \
    } while (0)

#endif