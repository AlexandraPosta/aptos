#ifndef ROTATION_H
#define ROTATION_H

#include <Arduino.h>
#include <math.h>

struct Quaternion
{
    float w, x, y, z;
    void toEuler(float &roll, float &pitch, float &yaw);
};

#endif