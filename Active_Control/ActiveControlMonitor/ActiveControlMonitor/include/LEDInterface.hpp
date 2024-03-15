#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H

#include <Arduino.h>
#include <elapsedMillis.h>
#define LED_PIN 6
namespace LED
{
    enum class LEDProgram
    {
        Off,
        On,
        BlinkOne,
        BlinkTwo,
        BlinkThree,
        RapidBlink10s,
        ContinuousBlink,

    };

    class LEDInterface
    {
    public:
        void start(LEDProgram defaultProgram_);
        void update();
        void startProgram(LEDProgram program);
        void endProgram();

    private:
        LEDProgram currentProgram = LEDProgram::Off;
        LEDProgram defaultProgram = LEDProgram::Off;
        elapsedMillis elapsedTime;
        unsigned long prevTime;
        void blinkProgram(LEDProgram next);
    };
}

#endif