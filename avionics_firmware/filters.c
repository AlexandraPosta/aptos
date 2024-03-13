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

// Fixed-point arctangent function, returns degrees in fixed point
fixed_point_t atan2_fixed(fixed_point_t y, fixed_point_t x) {
    const fixed_point_t PI_DIV_4 = M_PI / 4;
    const fixed_point_t RAD_TO_DEG = 0x005748B3; // 180/π in Q15.16 format

    fixed_point_t abs_y = y < 0 ? -y : y;
    fixed_point_t angle;

    if (x >= 0) {
        fixed_point_t r = (x - abs_y) / (x + abs_y);
        angle = PI_DIV_4 - (r >> 1);
    } else {
        fixed_point_t r = (x + abs_y) / (abs_y - x);
        angle = 3 * PI_DIV_4 - (r >> 1);
    }
    angle = y < 0 ? -angle : angle;
    
    // Convert radians to degrees
    return (angle * RAD_TO_DEG) >> 16;
}

// Fixed-point square root function
fixed_point_t sqrt_fixed(fixed_point_t x){
    int32_t guess = 1 << 16; // 1 in fixed-point representation
    int32_t delta;

    do {
        delta = (x - (guess * guess)) / (2 * guess);
        guess += delta;
    } while (delta != 0);

    return guess;
}