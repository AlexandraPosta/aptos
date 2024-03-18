#ifndef SENSOR_INTERFACE_H
#define SENSOR_INTERFACE_H

#include <Arduino.h>
#include <LSM9DS1.h>
#include <Adafruit_BMP3XX.h>
#include <Wire.h>
#include <math.h>
#include "Logger.hpp"

#include "Rotation.hpp"
#include "ValueDefinitions.hpp"

#define NUM_GYRO_CALIB_READINGS 200
#define NUM_ACCEL_CALIB_READINGS 200
#define NUM_LAUNCH_ROD_ORIENT_READINGS 200

#define SEALEVELPRESSURE_HPA (1013.25)
#define NUM_BAROMETER_CALIB_READINGS 20

namespace SensorInterface
{

    enum class SensorInterfaceError : uint8_t
    {
        None,
        IMUStartUpError,
        BarometerStartUpError,
        BarometerReadError,
    };

    // Define the gyroscope measurement struct
    struct GyroMeasurement
    {
        float x, y, z;
    };

    struct AccelMeasurement
    {
        float x, y, z;
    };

    class SensorInterface
    {
    private:
        Adafruit_BMP3XX bmp;

        Quaternion currentOrientation{1, 0, 0, 0};
        Quaternion currentOrientationRate{0, 0, 0, 0};
        Quaternion launchRodOrientation{1, 0, 0, 0};
        GyroMeasurement gyroValues{0.0, 0.0, 0.0};
        GyroMeasurement gyroOffset{0.0, 0.0, 0.0};
        AccelMeasurement accelValues{0.0, 0.0, 0.0};
        AccelMeasurement accelOffset{0.0, 0.0, 0.0};
        float altitude;
        float prevAltitude;
        float zeroAltitude = 0;
        float verticalVelocity = 0;
        bool orientationUpdated = false;
        bool accelUpdated = false;
        bool altitudeUpdated = false;
        void updateOrientation(unsigned long dt);
        uint8_t apogeeCounter;
        uint8_t landingCounter;
        void changeCoordinateSystem(float &x, float &y, float &z);
        Logger::Logger &logger;

    public:
        SensorInterfaceError error = SensorInterfaceError::None;
        SensorInterface(Logger::Logger &logger_) : logger{logger_} {};
        void start();
        bool wasOrientationUpdated();
        bool wasAccelUpdated();
        bool wasBarometerUpdated();
        void update(unsigned long dt);
        void calculateLaunchRodOrientation(float &roll, float &pitch, float &yaw);
        void calculateLaunchRodAltitude(float &altitude_);
        void calibrateGyroscope(float &gx, float &gy, float &gz);
        void calibrateAccelerometer(float &ax, float &ay, float &az);
        void resetToLaunchRodOrientation();
        float angleToVertical();

        void getLastGyroReading(float &x, float &y, float &z);
        void getLastAccelReading(float &x, float &y, float &z);
        void getLastAltitudeReading(float &alt);
        void getLastVelocityReading(float &vel);
        void getLastOrientation(float &w, float &x, float &y, float &z);
        void resetOrientation();
        bool detectLaunch();
    };

}

#endif