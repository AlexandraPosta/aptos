#ifndef VALUE_DEFINITIONS
#define VALUE_DEFINITIONS

#define DEBUG_NO_SD 0
// CS pins
#define BMP_CS_PIN 8
#define SD_CS_PIN 10

#define MAX_ALLOWABLE_ORIENTATION_ANGLE 0.349 // rad
#define MAX_ARM_WAIT_TIME 1200000             // ms
#define LAUNCH_ACCEL_MAGNITUDE_G 4            // g

// check intervals
#define FLUSH_SD_INTERVAL 200 // ms

// number of readings before apogee or landing detection
// #define APOGEE_COUNTS 5
// #define LANDING_COUNTS 5

// watchdog timer values
#define WDT_TIMEOUT 500
#define WDT_TRIGGER 0
#define WDT_WINDOW 0

#define DEBUG_NO_PRIMARY 1
#define DEBUG_ACTIVE_CONTROL_STATE 1

// #define PI 3.14159
// #define FILTER_MODE 2 // 0: No Filter; 1: Complementary; 2: Cascaded Complementary

// Complementary filter values
// #if FILTER_MODE == 1
// #define GYRO_WEIGHT 0.99
// #define ACCEL_WEIGHT 0.01
// #endif

// Cascaded Complementary filter
// #if FILTER_MODE == 2
// #define KP 1
// #define KI 1
// #define GYRO_WEIGHT 0.7
// #define ACCEL_WEIGHT 0.3
// #endif

#endif