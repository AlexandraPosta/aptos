#ifndef VALUE_DEFINITIONS
#define VALUE_DEFINITIONS

// lqr constants
#define STATE_SPACE_SIZE 6
#define NUM_SERVOS 4
#define NUM_GAINS 50

#define ZERO_DEFLECTIONS_FLIGHT 1
#define MAX_ALLOWABLE_ORIENTATION_ANGLE 0.349 // rad
#define MAX_DEFLECTION_RAD 0.20944            // rad
#define MAX_NEGATIVE_DEFLECTION_RAD -0.20944  // rad
#define MAX_ALLOWABLE_ZERO_ANGLE 0.0174       // rad
#define MAX_ALLOWABLE_DEFLECTION_ERROR 0.0174 // rad
#define MAX_ARM_WAIT_TIME 2000                // ms
#define LAUNCH_ACCEL_MAGNITUDE_MS2 50         // ms-2
#define V_MC 30                               // ms-1
#define V_MAX 120

// check intervals
#define GAIN_UPDATE_INTERVAL 10      // in ms
#define MONITOR_CHECK_INTERVAL 50    // in ms
#define READ_DEFLECTIONS_INTERVAL 50 // in ms
#define READ_VOLTAGE_INTERVAL 100    // in ms
#define FLUSH_SD_INTERVAL 200        // in ms

// number of readings before apogee or landing detection
#define APOGEE_COUNTS 5
#define LANDING_COUNTS 5

// debug values
#define DEBUG_ACTIVE_CONTROL_STATE 0
#define DEBUG_ACTIVE_CONTROL_VELOCITY 50
#define DEBUG_NO_MONITOR 1
#define DEBUG_NO_SERVOS 1

// watchdog timer values
#define WDT_TIMEOUT 2000
#define WDT_TRIGGER 2000
#define WDT_WINDOW 0

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

struct GainVariables
{
    float velocity;
};

#endif