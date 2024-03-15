#ifndef LQR_CONTROLLER_H
#define LQR_CONTROLLER_H

#include "ValueDefinitions.hpp"

class LQRController
{
private:
    float *currentGain;
    int currentGainIndex = 0;
    float gain[STATE_SPACE_SIZE * NUM_SERVOS];
    float availableGains[NUM_GAINS * STATE_SPACE_SIZE * NUM_SERVOS];
    float avGains[NUM_GAINS][NUM_SERVOS][STATE_SPACE_SIZE] =
        {
            {
                {0.0},
                {0.0},
                {0.0},
                {0.0},
            },
            {
                {0.0},
                {0.0},
                {0.0},
                {0.0},
            }};
    float zeroGain[STATE_SPACE_SIZE * NUM_SERVOS] = {0.0f};

public:
    LQRController();
    void start();
    void updateGain(float stateSpace[STATE_SPACE_SIZE], GainVariables &gainVars);
    void performLQR(float stateSpace[STATE_SPACE_SIZE], float calculatedDeflections[NUM_SERVOS]);
    void printCurrentGain();
};

#endif