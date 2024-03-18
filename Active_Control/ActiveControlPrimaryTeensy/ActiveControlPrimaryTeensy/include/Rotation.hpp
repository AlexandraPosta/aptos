#ifndef Rotation_H
#define Rotation_H

#include <Arduino.h>
#include <math.h>

struct Euler;
struct Quaternion;

struct Euler
{
    float roll, pitch, yaw;

    void toQuaternion(Quaternion &q);
};

struct Quaternion
{
    float w, x, y, z;

    void toEuler(float &roll, float &pitch, float &yaw);
    void toEuler(Euler &euler);
};

#endif