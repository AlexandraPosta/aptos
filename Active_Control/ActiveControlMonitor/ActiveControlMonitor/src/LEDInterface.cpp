#include "LEDInterface.hpp"
#include "BuzzerInterface.hpp"

void LED::LEDInterface::start(LEDProgram defaultProgram_)
{
    defaultProgram = defaultProgram_;
    currentProgram = defaultProgram_;
    pinMode(LED_PIN, OUTPUT);
}

void LED::LEDInterface::update()
{
    switch (currentProgram)
    {
    case LED::LEDProgram::Off:
    {
        digitalWrite(LED_PIN, LOW);
        break;
    }
    case LED::LEDProgram::On:
    {
        digitalWrite(LED_PIN, HIGH);
        break;
    }
    case LED::LEDProgram::RapidBlink10s:
    {
        if (elapsedTime > 10000)
        {
            endProgram();
        }
        if ((elapsedTime / 100) % 2 == 0)
        {
            digitalWrite(LED_PIN, HIGH);
        }
        else
        {
            digitalWrite(LED_PIN, LOW);
        }
        break;
    }
    case LED::LEDProgram::ContinuousBlink:
    {

        if ((elapsedTime / 100) % 2 == 0)
        {
            digitalWrite(LED_PIN, HIGH);
        }
        else
        {
            digitalWrite(LED_PIN, LOW);
        }
        break;
    }
    case LED::LEDProgram::BlinkThree:
    {
        blinkProgram(LED::LEDProgram::BlinkTwo);
        break;
    }
    case LED::LEDProgram::BlinkTwo:
    {
        blinkProgram(LED::LEDProgram::BlinkOne);
        break;
    }
    case LED::LEDProgram::BlinkOne:
    {
        blinkProgram(defaultProgram);
        break;
    }
    default:
        break;
    }
}

void LED::LEDInterface::startProgram(LEDProgram program)
{
    elapsedTime = 0;
    currentProgram = program;
}

void LED::LEDInterface::endProgram()
{
    startProgram(defaultProgram);
}

void LED::LEDInterface::blinkProgram(LEDProgram next)
{
    if (elapsedTime > 1000)
    {
        startProgram(next);
    }
    else if (elapsedTime > 500)
    {
        digitalWrite(LED_PIN, LOW);
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
    }
}
