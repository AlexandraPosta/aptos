#include "SensorInterface.hpp"

void SensorInterface::SensorInterface::start()
{
    error = SensorInterfaceError::None;
    if (!icm.begin_SPI(IMU_CS_PIN))
    {
        error = SensorInterfaceError::IMUStartUpError;
        return;
    }
    icm.setAccelRange(IMU_ACCEL_RANGE);
    icm.setGyroRange(IMU_GYRO_RANGE);
    // icm.setGyroRateDivisor
    // icm.
    gyroSensor = icm.getGyroSensor();
    accelSensor = icm.getAccelerometerSensor();

    if (!bmp.begin_SPI(BMP_CS_PIN))
    {
        error = SensorInterfaceError::BarometerStartUpError;
        return;
    }
    // Set up oversampling and filter initialization
    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

bool SensorInterface::SensorInterface::wasOrientationUpdated()
{
    return orientationUpdated;
}

bool SensorInterface::SensorInterface::wasAccelUpdated()
{
    return accelUpdated;
}

bool SensorInterface::SensorInterface::wasBarometerUpdated()
{
    return altitudeUpdated;
}

void SensorInterface::SensorInterface::updateOrientation(unsigned long dt)
{
    float wx = gyroValues.x;
    float wy = gyroValues.y;
    float wz = gyroValues.z;
    float qw = currentOrientation.w;
    float qx = currentOrientation.x;
    float qy = currentOrientation.y;
    float qz = currentOrientation.z;

    currentOrientationRate = {0.5f * (-wx * qx - wy * qy - wz * qz),
                              0.5f * (wx * qw + wz * qy - wy * qz),
                              0.5f * (wy * qw - wz * qx + wx * qz),
                              0.5f * (wz * qw + wy * qx - wx * qy)};

    // calculate derivative in rad/ns
    // currentOrientationRate = {0.5e-6f * qDot.w, 0.5e-6f * qDot.x, 0.5e-6f * qDot.y, 0.5e-6f * qDot.z};

    // Update the quaternion orientation using the derivative and timestep
    currentOrientation.w += currentOrientationRate.w * dt * 1e-6f;
    currentOrientation.x += currentOrientationRate.x * dt * 1e-6f;
    currentOrientation.y += currentOrientationRate.y * dt * 1e-6f;
    currentOrientation.z += currentOrientationRate.z * dt * 1e-6f;

    float norm = sqrt(currentOrientation.w * currentOrientation.w +
                      currentOrientation.x * currentOrientation.x +
                      currentOrientation.y * currentOrientation.y +
                      currentOrientation.z * currentOrientation.z);

    currentOrientation.w /= norm;
    currentOrientation.x /= norm;
    currentOrientation.y /= norm;
    currentOrientation.z /= norm;

    prevOrientationEuler = currentOrientationEuler;
    currentOrientation.toEuler(currentOrientationEuler);
    if ((currentOrientationEuler.roll < (-(PI) + 0.6)) & (prevOrientationEuler.roll > ((PI)-0.6)))
    {
        currentOrientationRateEuler.roll = 1e-6 * (currentOrientationEuler.roll + 2 * PI - prevOrientationEuler.roll) / dt;
    }
    else
    {
        currentOrientationRateEuler.roll = 1e-6 * (currentOrientationEuler.roll - prevOrientationEuler.roll) / dt;
    }
}

void SensorInterface::SensorInterface::changeCoordinateSystem(float &x, float &y, float &z)
{
    float tempX = x;
    x = z;
    y = y;
    z = -tempX;
}

void SensorInterface::SensorInterface::update(unsigned long dt)
{
    orientationUpdated = false;

    error = SensorInterfaceError::None;

    // logger.logDebug("Getting gyro");
    icm.getEvent(&accelEvent, &gyroEvent, &tempEvent);

    // logger.logDebug("Got gyro");
    // delay(100);
    if (true)
    {
        // logger.logDebug("Updating orientation");
        gyroValues.x = gyroEvent.gyro.x;
        gyroValues.y = gyroEvent.gyro.y;
        gyroValues.z = gyroEvent.gyro.z;
        changeCoordinateSystem(gyroValues.x, gyroValues.y, gyroValues.z);
        gyroValues = {gyroValues.x - gyroOffset.x, gyroValues.y - gyroOffset.y, gyroValues.z - gyroOffset.z};
        // gyroValues.x -= 10.0f;
        // gyroValues.y -= 10.f;
        // gyroValues.z -= 10.f;
        updateOrientation(dt);
        if (error != SensorInterfaceError::None)
        {
            return;
        }
        orientationUpdated = true;
        // logger.logDebug("Updated orientation");
    }

    // static bool accelSuccess = accelSensor->getEvent(&accelEvent);
    accelUpdated = false;
    if (true)
    {
        changeCoordinateSystem(accelEvent.acceleration.x, accelEvent.acceleration.y, accelEvent.acceleration.z);
        accelValues = {accelEvent.acceleration.x - accelOffset.x, accelEvent.acceleration.y - accelOffset.y, accelEvent.acceleration.z - accelOffset.z};
        accelUpdated = true;
    }

    altitudeUpdated = false;
    if (!bmp.performReading())
    {
        error = SensorInterfaceError::BarometerReadError;
        return;
    }

    altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA) - zeroAltitude;
    verticalVelocity = (altitude - prevAltitude) * dt * 1e-6f;
    prevAltitude = altitude;
    altitudeUpdated = true;
}

void SensorInterface::SensorInterface::calculateLaunchRodOrientation(float &roll, float &pitch, float &yaw)
{

    AccelMeasurement averageReadings{0.0, 0.0, 0.0};
    AccelMeasurement accel;
    int numReadings = 0;
    icm.getEvent(&accelEvent, &gyroEvent, &tempEvent);
    while (numReadings < NUM_LAUNCH_ROD_ORIENT_READINGS)
    {
        if (true)
        {
            accel.x = accelEvent.acceleration.x;
            accel.y = accelEvent.acceleration.y;
            accel.z = accelEvent.acceleration.z;
            changeCoordinateSystem(accel.x, accel.y, accel.z);

            numReadings++;
            averageReadings.x += accel.x - accelOffset.x;
            averageReadings.y += accel.y - accelOffset.y;
            averageReadings.z += accel.z - accelOffset.z;
        }
        delay(50);
    }
    averageReadings.x /= NUM_LAUNCH_ROD_ORIENT_READINGS;
    averageReadings.y /= NUM_LAUNCH_ROD_ORIENT_READINGS;
    averageReadings.z /= NUM_LAUNCH_ROD_ORIENT_READINGS;
    float g = sqrt((averageReadings.x * averageReadings.x) + (averageReadings.y * averageReadings.y) + (averageReadings.z * averageReadings.z));
    logger.logDebug("G: " + String(g, 5));
    averageReadings.x /= g;
    averageReadings.y /= g;
    averageReadings.z /= g;
    logger.logDebug("ax: " + String(averageReadings.x, 5));
    logger.logDebug("ay: " + String(averageReadings.y, 5));
    logger.logDebug("az: " + String(averageReadings.z, 5));

    AccelMeasurement tempReadings;
    tempReadings.x = averageReadings.x;
    tempReadings.y = averageReadings.y;
    tempReadings.z = averageReadings.z;

    if (tempReadings.x >= 0)
    {
        launchRodOrientation.w = -sqrt(0.5 * (tempReadings.x + 1));
        launchRodOrientation.x = 0.0;
        launchRodOrientation.y = -tempReadings.z / (sqrt(2 * (tempReadings.x + 1)));
        launchRodOrientation.z = -(-tempReadings.y / (sqrt(2 * (tempReadings.x + 1))));
    }
    else if (tempReadings.x < 0)
    {
        logger.logError("Module is upside down!");
    }
    launchRodOrientation.toEuler(roll, pitch, yaw);
}

void SensorInterface::SensorInterface::calculateLaunchRodAltitude(float &altitude_)
{
    zeroAltitude = 0;
    for (int i = 1; i <= NUM_BAROMETER_CALIB_READINGS; i++)
    {
        if (!bmp.performReading())
        {
            continue;
        }
        zeroAltitude = (zeroAltitude * (i - 1) + bmp.readAltitude(SEALEVELPRESSURE_HPA)) / i;
        delay(100);
    }
    altitude_ = zeroAltitude;
}

void SensorInterface::SensorInterface::calibrateGyroscope(float &gx, float &gy, float &gz)
{
    gyroOffset = {0.0, 0.0, 0.0};
    int numReadings = 1;
    icm.getEvent(&accelEvent, &gyroEvent, &tempEvent);
    while (numReadings <= NUM_GYRO_CALIB_READINGS)
    {

        if (true)
        {

            gyroValues.x = gyroEvent.gyro.x;
            gyroValues.y = gyroEvent.gyro.y;
            gyroValues.z = gyroEvent.gyro.z;
            changeCoordinateSystem(gyroValues.x, gyroValues.y, gyroValues.z);
            numReadings++;
            gyroOffset.x = (gyroOffset.x * (numReadings - 1) + gyroValues.x) / numReadings;
            gyroOffset.y = (gyroOffset.y * (numReadings - 1) + gyroValues.y) / numReadings;
            gyroOffset.z = (gyroOffset.z * (numReadings - 1) + gyroValues.z) / numReadings;
        }
        delay(100);
    }
    gx = gyroOffset.x;
    gy = gyroOffset.y;
    gz = gyroOffset.z;
}

// void SensorInterface::SensorInterface::calibrateGyroscope()
// {
//     gyroOffset = {0.0, 0.0, 0.0};
//     int numReadings = 0;
//     icm.getEvent(&accelEvent, &gyroEvent, &tempEvent);
//     while (numReadings < NUM_GYRO_CALIB_READINGS)
//     {
//         if (true)
//         {

//             gyroValues.x = gyroEvent.gyro.x;
//             gyroValues.y = gyroEvent.gyro.y;
//             gyroValues.z = gyroEvent.gyro.z;
//             changeCoordinateSystem(gyroValues.x, gyroValues.y, gyroValues.z);
//             numReadings++;
//             gyroOffset.x = (gyroOffset.x * (numReadings - 1) + gyroEvent.gyro.x) / numReadings;
//             gyroOffset.y = (gyroOffset.y * (numReadings - 1) + gyroEvent.gyro.y) / numReadings;
//             gyroOffset.z = (gyroOffset.z * (numReadings - 1) + gyroEvent.gyro.z) / numReadings;
//         }
//         delay(100);
//     }
// }

void SensorInterface::SensorInterface::calibrateAccelerometer(float &ax, float &ay, float &az)
{
    AccelMeasurement accel;
    accelOffset = {0.0, 0.0, 0.0};
    int numReadings = 1;
    icm.getEvent(&accelEvent, &gyroEvent, &tempEvent);
    while (numReadings <= NUM_ACCEL_CALIB_READINGS)

    {
        if (true)
        {
            accel.x = accelEvent.acceleration.x;
            accel.y = accelEvent.acceleration.y;
            accel.z = accelEvent.acceleration.z;
            changeCoordinateSystem(accel.x, accel.y, accel.z);
            numReadings++;
            accelOffset.x = (accelOffset.x * (numReadings - 1) + accel.x) / numReadings;
            accelOffset.y = (accelOffset.y * (numReadings - 1) + accel.y) / numReadings;
            accelOffset.z = (accelOffset.z * (numReadings - 1) + accel.z) / numReadings;
        }
        delay(50);
    }
    logger.logDebug("ax average: " + String(accelOffset.x, 5));
    accelOffset.x -= 9.81;
    ax = accelOffset.x;
    ay = accelOffset.y;
    az = accelOffset.z;
}

void SensorInterface::SensorInterface::resetToLaunchRodOrientation()
{
    currentOrientation = launchRodOrientation;
}

float SensorInterface::SensorInterface::angleToVertical()
{
    float R00 = 1 - 2 * (currentOrientation.y * currentOrientation.y + currentOrientation.z * currentOrientation.z);

    float R10 = 2 * (currentOrientation.x * currentOrientation.y + currentOrientation.z * currentOrientation.w);

    float R20 = 2 * (currentOrientation.x * currentOrientation.z - currentOrientation.y * currentOrientation.w);

    // Calculate the global vertical vector
    static const float vertical[3] = {1.0, 0.0, 0.0};

    // Calculate the x axis of the rocket in the global frame
    float xRocket[3] = {R00, R10, R20};

    // Calculate the angle between the x axis of the rocket and the global vertical axis
    float angle = acos(vertical[0] * xRocket[0] + vertical[1] * xRocket[1] + vertical[2] * xRocket[2]);

    return angle;
}

void SensorInterface::SensorInterface::setStateSpace(float stateSpace[STATE_SPACE_SIZE])
{
    // currentOrientation.toEuler(currentOrientationEuler);
    // currentOrientationRate.toEuler(currentOrientationRateEuler);
    stateSpace[0] = currentOrientationEuler.roll;
    stateSpace[1] = currentOrientationEuler.pitch;
    stateSpace[2] = currentOrientationEuler.yaw;
    stateSpace[3] = currentOrientationRateEuler.roll;
    stateSpace[4] = currentOrientationRateEuler.pitch;
    stateSpace[5] = currentOrientationRateEuler.yaw;
}

void SensorInterface::SensorInterface::getLastGyroReading(float &x, float &y, float &z)
{
    x = gyroValues.x;
    y = gyroValues.y;
    z = gyroValues.z;
}

void SensorInterface::SensorInterface::getLastAccelReading(float &x, float &y, float &z)
{
    x = accelValues.x;
    y = accelValues.y;
    z = accelValues.z;
}

void SensorInterface::SensorInterface::getLastAltitudeReading(float &alt)
{
    alt = altitude;
}

void SensorInterface::SensorInterface::getLastVelocityReading(float &vel)
{
    vel = verticalVelocity;
}

void SensorInterface::SensorInterface::getLastOrientation(float &w, float &x, float &y, float &z)
{
    w = currentOrientation.w;
    x = currentOrientation.x;
    y = currentOrientation.y;
    z = currentOrientation.z;
}

void SensorInterface::SensorInterface::getLastOrientationRate(float &w, float &x, float &y, float &z)
{
    w = currentOrientationRate.w;
    x = currentOrientationRate.x;
    y = currentOrientationRate.y;
    z = currentOrientationRate.z;
}

bool SensorInterface::SensorInterface::detectLaunch()
{
    float accelMag = sqrt((accelValues.x * accelValues.x) + (accelValues.y * accelValues.y) + accelValues.z * accelValues.z);
    if (accelMag > LAUNCH_ACCEL_MAGNITUDE_MS2)
    {
        return true;
    }
    return false;
}

bool SensorInterface::SensorInterface::detectApogee()
{
    if (prevAltitude > altitude)
    {
        apogeeCounter++;
    }
    else
    {
        apogeeCounter = 0;
        return false;
    }
    if (apogeeCounter >= APOGEE_COUNTS)
    {
        return true;
    }
}

bool SensorInterface::SensorInterface::detectLanding()
{
    if (abs(prevAltitude - altitude) < 0.5)
    {
        landingCounter++;
    }
    else
    {
        landingCounter = 0;
        return false;
    }
    if (landingCounter >= LANDING_COUNTS)
    {
        return true;
    }
}

void SensorInterface::SensorInterface::resetOrientation()
{
    currentOrientation = {1, 0, 0, 0};
    currentOrientationEuler = {0, 0, 0};
    currentOrientationRate = {0, 0, 0, 0};
    currentOrientationRateEuler = {0, 0, 0};
    prevAltitude = 0;
    verticalVelocity = 0;
}
