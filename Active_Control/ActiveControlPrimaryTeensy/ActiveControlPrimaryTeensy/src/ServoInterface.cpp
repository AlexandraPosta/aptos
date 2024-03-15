#include "ServoInterface.hpp"

bool Servo::ServoInterface::checkZeroDeflection()
{
#if DEBUG_NO_SERVOS
    return true;
#endif
    float actualDeflections[NUM_SERVOS] = {2.0};
    getDeflections(actualDeflections);
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        if ((actualDeflections[i] > MAX_ALLOWABLE_ZERO_ANGLE) | (actualDeflections[i] < -MAX_ALLOWABLE_ZERO_ANGLE))
        {
            return false;
        }
    }
    return true;
}

bool Servo::ServoInterface::checkDeflections(float deflection)
{
#if DEBUG_NO_SERVOS
    return true;
#endif
    float actualDeflections[NUM_SERVOS] = {2.0};
    getDeflections(actualDeflections);
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        if ((actualDeflections[i] > deflection + MAX_ALLOWABLE_DEFLECTION_ERROR) | (actualDeflections[i] < deflection - MAX_ALLOWABLE_DEFLECTION_ERROR))
        {
            return false;
        }
    }
    return true;
}

void Servo::ServoInterface::setPID(uint16_t Kp, uint16_t Ki, uint16_t Kd)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        setPIDSingle(i, Kp, Ki, Kd);
    }
}
void Servo::ServoInterface::setPIDSingle(uint8_t servoNum, uint16_t Kp, uint16_t Ki, uint16_t Kd)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    if (servoNum >= NUM_SERVOS)
    {
        return;
    }
    servos[servoNum].writeRam2(HerkulexRamRegister::PositionKp, Kp);
    servos[servoNum].writeRam2(HerkulexRamRegister::PositionKi, Ki);
    servos[servoNum].writeRam2(HerkulexRamRegister::PositionKd, Kd);
}

void Servo::ServoInterface::getPIDs(uint16_t Kps[NUM_SERVOS], uint16_t Kis[NUM_SERVOS], uint16_t Kds[NUM_SERVOS])
{
#if DEBUG_NO_SERVOS
    return;
#endif
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        getPID(i, Kps[i], Kis[i], Kds[i]);
    }
}

void Servo::ServoInterface::getPID(uint8_t servoNum, uint16_t &Kp, uint16_t &Ki, uint16_t &Kd)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    Kp = servos[servoNum].readRam2(HerkulexRamRegister::PositionKp);
    Ki = servos[servoNum].readRam2(HerkulexRamRegister::PositionKi);
    Kd = servos[servoNum].readRam2(HerkulexRamRegister::PositionKd);
}

void Servo::ServoInterface::setPIDEEP(uint16_t Kp, uint16_t Ki, uint16_t Kd)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        setPIDEEPSingle(i, Kp, Ki, Kd);
    }
}
void Servo::ServoInterface::setPIDEEPSingle(uint8_t servoNum, uint16_t Kp, uint16_t Ki, uint16_t Kd)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    if (servoNum >= NUM_SERVOS)
    {
        return;
    }
    servos[servoNum].writeEep2(HerkulexEepRegister::PositionKp, Kp);
    servos[servoNum].writeEep2(HerkulexEepRegister::PositionKi, Ki);
    servos[servoNum].writeEep2(HerkulexEepRegister::PositionKd, Kd);
}

void Servo::ServoInterface::resetPID()
{
#if DEBUG_NO_SERVOS
    return;
#endif
    uint16_t Kp, Ki, Kd;
    for (int i = 0; i < NUM_SERVOS; i++)
    {
        Kp = servos[i].readEep2(HerkulexEepRegister::PositionKp);
        Ki = servos[i].readEep2(HerkulexEepRegister::PositionKp);
        Kd = servos[i].readEep2(HerkulexEepRegister::PositionKp);
        setPIDSingle(i, Kp, Ki, Kd);
    }
}

int16_t Servo::ServoInterface::addZeroOffset(uint8_t servoNum)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    zeroOffset[servoNum]++;
    return zeroOffset[servoNum];
}

int16_t Servo::ServoInterface::subZeroOffset(uint8_t servoNum)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    zeroOffset[servoNum]--;
    return zeroOffset[servoNum];
}

void Servo::ServoInterface::quickStartAndDeflect()
{
#if DEBUG_NO_SERVOS
    return;
#endif
    servoSerial.begin(SERVO_BAUD);
    setTorquesOn();
    setZeroDeflections();
}

void Servo::ServoInterface::start()
{
}

void Servo::ServoInterface::setTorquesOn()
{
#if DEBUG_NO_SERVOS
    return;
#endif
    servoX1.setTorqueOn();
    servoX2.setTorqueOn();
    servoY1.setTorqueOn();
    servoY2.setTorqueOn();
}

void Servo::ServoInterface::setTorquesOff()
{
#if DEBUG_NO_SERVOS
    return;
#endif
    servoX1.setTorqueOff();
    servoX2.setTorqueOff();
    servoY1.setTorqueOff();
    servoY2.setTorqueOff();
}

void Servo::ServoInterface::getDeflections(float actualDeflections[4])
{
#if DEBUG_NO_SERVOS
    return;
#endif
    actualDeflections[0] = intToRads(servoX1.getPosition());
    actualDeflections[1] = intToRads(servoX2.getPosition());
    actualDeflections[2] = intToRads(servoY1.getPosition());
    actualDeflections[3] = intToRads(servoY2.getPosition());
}

void Servo::ServoInterface::setDeflections(float deflections[4])
{
#if DEBUG_NO_SERVOS
    return;
#endif
#if ZERO_DEFLECTIONS_FLIGHT
    setZeroDeflections();
#else
    switch (currentMode)
    {
    case ServoMode::Disabled:
        return;
        break;

    case ServoMode::ZeroDeflection:
        setZeroDeflections();
        break;

    case ServoMode::Enabled:
        for (int i = 0; i < NUM_SERVOS; i++)
        {
            if (deflections[i] > MAX_DEFLECTION_RAD)
            {
                deflections[i] = MAX_DEFLECTION_RAD;
            }
            else if (deflections[i] < -MAX_DEFLECTION_RAD)
            {
                deflections[i] = -MAX_DEFLECTION_RAD;
            }
        }
        servoBus.prepareSynchronizedMove(0);
        servoX1.setPosition(radsToInt(deflections[0]));
        servoX2.setPosition(radsToInt(deflections[1]));
        servoY1.setPosition(radsToInt(deflections[2]));
        servoY2.setPosition(radsToInt(deflections[3]));
        servoBus.executeMove();
        break;
    }
#endif
}

void Servo::ServoInterface::setMode(ServoMode mode)
{
#if DEBUG_NO_SERVOS
    return;
#endif
    currentMode = mode;
    switch (mode)
    {
    case ServoMode::Disabled:
        setTorquesOff();
        break;
    case ServoMode::ZeroDeflection:
        setZeroDeflections();
        break;
    case ServoMode::Enabled:
        break;
    }
}

void Servo::ServoInterface::setZeroDeflections()
{
#if DEBUG_NO_SERVOS
    return;
#endif
    servoBus.prepareSynchronizedMove(0);
    servoX1.setPosition(512 + zeroOffset[0]);
    servoX2.setPosition(512 + zeroOffset[1]);
    servoY1.setPosition(512 + zeroOffset[2]);
    servoY2.setPosition(512 + zeroOffset[3]);
    servoBus.executeMove();
}