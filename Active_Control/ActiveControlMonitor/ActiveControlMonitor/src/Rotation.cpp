#include "Rotation.hpp"

void Quaternion::toEuler(float &roll, float &pitch, float &yaw)
{

    // XYZ order
    float qw2 = w * w;
    float qx2 = x * x;
    float qy2 = y * y;
    float qz2 = z * z;

    float dcm32 = 2 * (y * z - x * w);
    float dcm33 = qw2 - qx2 - qy2 + qz2;
    float dcm31 = 2 * (x * z + y * w);
    float dcm21 = 2 * (x * y - z * w);
    float dcm11 = qw2 + qx2 - qy2 - qz2;

    roll = atan2(-dcm32, dcm33);
    pitch = asin(dcm31);
    yaw = atan2(-dcm21, dcm11);
}