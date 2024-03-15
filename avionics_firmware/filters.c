/*
    Leeds University Rocketry Organisation - LURA
    Author Name: Oliver Martin
    Created on: 13 March 2024
    Description: Different data filter options for sensors
*/

#include "filters.h"

#pragma region simple low pass filters


/**
  @brief Simple first order low pass filter
  @param input new raw data value
  @param output_prev the previous output of this function
  @param alpha value between 0-100 which determines the cut-off frequency
  @return int32_t of the low pass output
  @note alpha can be calculated by a = (2pi*Fc)/(Fs+2pi*Fc), where Fc is cut-off frequency, Fs is sample frequency
*/
int32_t LPF1(int32_t input, int32_t output_prev, uint8_t alpha){
    //alpha is usually between 0-1 but we don't have floats. so alpha is between 0-100 and the result is divided by 100.
    //alpha * input + (1-alpha)*previous output
    return (alpha * input + (100 - alpha) * output_prev)/100;
}


#pragma endregion

#define ATAN2_PRECISION 1000 // Define the precision for the angle calculation (milli degrees)
//
int32_t atan2_milli(int32_t y, int32_t x) {
    // Constants for scaling
    const int32_t atan2_denominator = 1000000; // Denominator for scaling the fraction
    const int32_t atan2_max = 2000000; // Maximum value for scaling

    // Quadrant check and absolute value
    int8_t neg = (y < 0) ? -1 : 1;
    y = (y < 0) ? -y : y;

    int32_t angle = 0; // Initialize angle

    // Special cases
    if (x == 0 && y == 0) return 0; // Undefined, return 0

    if (x == 0 && y > 0) return 900000; // 90 degrees (in milli degrees)
    if (x == 0 && y < 0) return -900000; // -90 degrees (in milli degrees)

    if (y == 0 && x > 0) return 0; // 0 degrees
    if (y == 0 && x < 0) return 1800000; // 180 degrees

    // Compute the angle using a simple arctan approximation
    if (x >= 0) {
        angle = ((y * atan2_denominator) / (x + y));
    } else {
        angle = atan2_max - ((y * atan2_denominator) / (y - x));
    }

    // Quadrant adjustments
    if (x < 0) {
        angle += 900000; // 90 degrees
    } else if (y < 0) {
        angle += 1800000; // 180 degrees
    }

    // Adjust for negative values
    angle *= neg;

    return angle;
}

int32_t sqrt_int(int32_t x) {
    if (x <= 1) return x; // Base case for 0 and 1

    // Initial guess for the square root
    int32_t root = x / 2;
    int32_t root_prev = 0;

    // Iterate until convergence
    while (root != root_prev) {
        root_prev = root;
        root = (root + x / root) / 2; // Apply Newton's method
    }

    return root;
}