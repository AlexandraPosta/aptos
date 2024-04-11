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
