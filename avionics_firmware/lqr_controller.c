/*
  Leeds University Rocketry Organisation - LURA
  Author Name: Alexandra Posta
  Created on: 19 March 2024
  Description: Include LQR Controller source file
*/

#include "lqr_controller.h"	

int _ravel_index_2d(int i, int j)
{
    return i * STATE_SPACE_DIM + j;
}

int _ravel_index_3d(int i, int j, int k) {
    return i * STATE_SPACE_DIM * NUM_SERVOS + j * STATE_SPACE_DIM + k;
}

void LQR_init(LQR_controller* lqr) {
    // Set the zero gain array to zero
    for (uint8_t i = 0; i < sizeof(lqr->zero_gains); i++) {
        lqr->zero_gains[i] = 0;
    }

    // Initialise the current gain and index to zero
    lqr->current_gain = &lqr->zero_gains[0];
    lqr->current_gain_index = 0.0f;

    // Initialise average gains
    double _avg_gains[NUM_GAINS][NUM_SERVOS][STATE_SPACE_DIM] = {
        {
            {5.9761e-05, -0.37796, -1.1106e-15, 0.26723, -0.38847, -2.2002e-16},
            {5.9761e-05, 0.37796, 1.1899e-15, 0.26723, 0.38847, 2.4892e-16},
            {5.9761e-05, 1.3538e-15, -0.37796, 0.26723, 6.8727e-16, -0.38847},
            {5.9761e-05, -1.1147e-15, 0.37796, 0.26723, -5.3388e-16, 0.38847},
        },
        {
            {5.9761e-05, -0.37796, -1.1945e-15, 0.26723, -0.3698, -6.5912e-16},
            {5.9761e-05, 0.37796, 1.1945e-15, 0.26723, 0.3698, 8.5519e-16},
            {5.9761e-05, 1.1922e-15, -0.37796, 0.26723, -5.9471e-17, -0.3698},
            {5.9761e-05, -9.2426e-16, 0.37796, 0.26723, 4.3445e-16, 0.3698},
        },
        {
            {5.9761e-05, -0.37796, -2.541e-16, 0.26723, -0.35334, 3.2436e-16},
            {5.9761e-05, 0.37796, 1.0805e-16, 0.26723, 0.35334, -3.8215e-16},
            {5.9761e-05, 5.845e-16, -0.37796, 0.26723, 4.866e-16, -0.35334},
            {5.9761e-05, -7.2657e-16, 0.37796, 0.26723, -5.5427e-16, 0.35334},
        },
        {
            {5.9761e-05, -0.37796, -1.3501e-15, 0.26723, -0.33875, -9.539e-16},
            {5.9761e-05, 0.37796, 1.4925e-15, 0.26723, 0.33875, 1.0615e-15},
            {5.9761e-05, 1.3536e-15, -0.37796, 0.26723, 9.6825e-17, -0.33875},
            {5.9761e-05, -1.2494e-15, 0.37796, 0.26723, 7.6555e-17, 0.33875},
        },
        {
            {5.9761e-05, -0.37796, -1.0119e-15, 0.26723, -0.32576, -7.2631e-16},
            {5.9761e-05, 0.37796, 1.0119e-15, 0.26723, 0.32576, 6.664e-16},
            {5.9761e-05, 1.2302e-15, -0.37796, 0.26723, 1.2e-15, -0.32576},
            {5.9761e-05, -1.3735e-15, 0.37796, 0.26723, -1.3666e-15, 0.32576},
        },
        {
            {5.9761e-05, -0.37796, -3.1242e-16, 0.26723, -0.31414, 3.4562e-17},
            {5.9761e-05, 0.37796, 4.1238e-16, 0.26723, 0.31414, -3.5944e-17},
            {5.9761e-05, 3.9589e-16, -0.37796, 0.26723, 1.055e-16, -0.31414},
            {5.9761e-05, -3.8633e-16, 0.37796, 0.26723, 2.1815e-17, 0.31414},
        },
        {
            {5.9761e-05, -0.37796, 3.161e-16, 0.26724, -0.30369, 3.6091e-16},
            {5.9761e-05, 0.37796, -3.4891e-16, 0.26724, 0.30369, -4.0157e-16},
            {5.9761e-05, -2.117e-16, -0.37796, 0.26724, -2.862e-16, -0.30369},
            {5.9761e-05, -3.8504e-16, 0.37796, 0.26724, 1.1155e-16, 0.30369},
        },
        {
            {5.9761e-05, -0.37796, -1.257e-16, 0.26724, -0.29426, 2.9677e-17},
            {5.9761e-05, 0.37796, -2.48e-16, 0.26724, 0.29426, -2.2668e-16},
            {5.9761e-05, 3.3545e-17, -0.37796, 0.26724, 1.2343e-16, -0.29426},
            {5.9761e-05, 8.711e-17, 0.37796, 0.26724, -5.8304e-17, 0.29426},
        },
        {
            {5.9761e-05, -0.37796, -5.7126e-16, 0.26724, -0.28573, -1.9429e-16},
            {5.9761e-05, 0.37796, 2.3604e-16, 0.26724, 0.28573, -8.9557e-17},
            {5.9761e-05, 2.7781e-16, -0.37796, 0.26724, 3.0401e-16, -0.28573},
            {5.9761e-05, -3.1431e-16, 0.37796, 0.26724, -2.7998e-16, 0.28573},
        },
        {
            {5.9761e-05, -0.37796, -4.2613e-16, 0.26724, -0.27799, -3.464e-16},
            {5.9761e-05, 0.37796, 5.9041e-16, 0.26724, 0.27799, 4.735e-16},
            {5.9761e-05, -3.791e-17, -0.37796, 0.26724, -6.3698e-16, -0.27799},
            {5.9761e-05, -1.9494e-16, 0.37796, 0.26724, 4.6164e-16, 0.27799},
        },
        {
            {5.9761e-05, -0.37796, -4.9524e-16, 0.26724, -0.27093, 1.1004e-16},
            {5.9761e-05, 0.37796, 6.1816e-16, 0.26724, 0.27093, 5.7007e-17},
            {5.9761e-05, 3.714e-16, -0.37796, 0.26724, 1.1956e-16, -0.27093},
            {5.9761e-05, -4.6425e-16, 0.37796, 0.26724, -1.9127e-16, 0.27093},
        },
        {
            {5.9761e-05, -0.37796, -1.945e-16, 0.26724, -0.26449, -1.7088e-16},
            {5.9761e-05, 0.37796, 4.6138e-17, 0.26724, 0.26449, -3.7306e-17},
            {5.9761e-05, 3.0252e-16, -0.37796, 0.26724, 3.6571e-16, -0.26449},
            {5.9761e-05, -1.6025e-16, 0.37796, 0.26724, -3.2465e-16, 0.26449},
        },
        {
            {5.9761e-05, -0.37796, -5.0142e-17, 0.26724, -0.2586, -2.1198e-16},
            {5.9761e-05, 0.37796, 5.8151e-17, 0.26724, 0.2586, 2.4594e-16},
            {5.9761e-05, 4.0491e-16, -0.37796, 0.26724, 3.1008e-17, -0.2586},
            {5.9761e-05, -3.589e-16, 0.37796, 0.26724, -1.9175e-17, 0.2586},
        },
        {
            {5.9761e-05, -0.37796, 6.5373e-16, 0.26724, -0.2532, 2.2251e-16},
            {5.9761e-05, 0.37796, -9.1472e-16, 0.26724, 0.2532, -4.8058e-16},
            {5.9761e-05, -9.3899e-16, -0.37796, 0.26724, -4.3971e-16, -0.2532},
            {5.9761e-05, 9.2892e-16, 0.37796, 0.26724, 5.6447e-16, 0.2532},
        },
        {
            {5.9761e-05, -0.37796, 2.3044e-16, 0.26724, -0.24823, 4.1821e-17},
            {5.9761e-05, 0.37796, -1.1762e-16, 0.26724, 0.24823, -1.5172e-17},
            {5.9761e-05, 5.0121e-18, -0.37796, 0.26724, -2.1533e-16, -0.24823},
            {5.9761e-05, 1.2597e-16, 0.37796, 0.26724, 3.2708e-16, 0.24823},
        },
        {
            {5.9761e-05, -0.37796, -7.9037e-17, 0.26724, -0.24366, 3.7992e-16},
            {5.9761e-05, 0.37796, 7.6733e-17, 0.26724, 0.24366, -2.2851e-16},
            {5.9761e-05, 6.0048e-16, -0.37796, 0.26724, 2.8873e-16, -0.24366},
            {5.9761e-05, -6.5322e-16, 0.37796, 0.26724, -3.5049e-16, 0.24366},
        },
        {
            {5.9761e-05, -0.37796, 3.967e-16, 0.26724, -0.23944, 1.7142e-16},
            {5.9761e-05, 0.37796, -4.8925e-16, 0.26724, 0.23944, -1.655e-16},
            {5.9761e-05, -4.9376e-16, -0.37796, 0.26724, -2.3671e-16, -0.23944},
            {5.9761e-05, 3.8968e-16, 0.37796, 0.26724, 1.7797e-16, 0.23944},
        },
        {
            {5.9761e-05, -0.37796, -3.6912e-16, 0.26724, -0.23554, 3.2987e-16},
            {5.9761e-05, 0.37796, 1.7657e-16, 0.26724, 0.23554, -4.8437e-16},
            {5.9761e-05, 6.9694e-16, -0.37796, 0.26724, 7.0032e-16, -0.23554},
            {5.9761e-05, -6.8332e-16, 0.37796, 0.26724, -6.2697e-16, 0.23554},
        },
        {
            {5.9761e-05, -0.37796, 3.4851e-16, 0.26724, -0.23194, -2.1889e-16},
            {5.9761e-05, 0.37796, -5.2277e-16, 0.26724, 0.23194, -1.1396e-16},
            {5.9761e-05, -4.9517e-16, -0.37796, 0.26724, -1.8321e-16, -0.23194},
            {5.9761e-05, 4.1859e-16, 0.37796, 0.26724, 1.1452e-16, 0.23194},
        },
        {
            {5.9761e-05, -0.37796, 4.0201e-16, 0.26724, -0.22859, 1.8597e-16},
            {5.9761e-05, 0.37796, -3.3882e-16, 0.26724, 0.22859, -9.7633e-17},
            {5.9761e-05, -5.7491e-17, -0.37796, 0.26724, 1.2955e-16, -0.22859},
            {5.9761e-05, 3.5633e-16, 0.37796, 0.26724, 6.819e-17, 0.22859},
        },
        {
            {5.9761e-05, -0.37796, 3.5127e-16, 0.26725, -0.22549, 6.4258e-17},
            {5.9761e-05, 0.37796, -3.1231e-16, 0.26725, 0.22549, -1.8752e-17},
            {5.9761e-05, -6.8925e-16, -0.37796, 0.26725, -2.4466e-16, -0.22549},
            {5.9761e-05, 7.4512e-16, 0.37796, 0.26725, 2.8411e-16, 0.22549},
        },
        {
            {5.9761e-05, -0.37796, 3.6176e-16, 0.26725, -0.22261, 7.6779e-17},
            {5.9761e-05, 0.37796, -5.201e-16, 0.26725, 0.22261, -1.1789e-16},
            {5.9761e-05, -4.7182e-16, -0.37796, 0.26725, -2.4959e-16, -0.22261},
            {5.9761e-05, 4.067e-16, 0.37796, 0.26725, 1.1337e-16, 0.22261},
        },
        {
            {5.9761e-05, -0.37796, 3.0274e-16, 0.26725, -0.21992, -5.6879e-17},
            {5.9761e-05, 0.37796, -1.0989e-16, 0.26725, 0.21992, 1.9312e-16},
            {5.9761e-05, -8.7544e-16, -0.37796, 0.26725, -4.1896e-16, -0.21992},
            {5.9761e-05, 1.0376e-15, 0.37796, 0.26725, 4.8957e-16, 0.21992},
        },
        {
            {5.9761e-05, -0.37796, 1.6384e-15, 0.26725, -0.21743, 5.5674e-16},
            {5.9761e-05, 0.37796, -6.8267e-16, 0.26725, 0.21743, -3.5555e-16},
            {5.9761e-05, -7.795e-16, -0.37796, 0.26725, -1.824e-16, -0.21743},
            {5.9761e-05, 8.6053e-16, 0.37796, 0.26725, 2.1196e-16, 0.21743},
        },
        {
            {5.9761e-05, -0.37796, 4.5545e-16, 0.26725, -0.2151, 1.0081e-16},
            {5.9761e-05, 0.37796, 1.3663e-16, 0.26725, 0.2151, 1.0469e-16},
            {5.9761e-05, -1.8759e-16, -0.37796, 0.26725, 1.5754e-16, -0.2151},
            {5.9761e-05, 1.2131e-16, 0.37796, 0.26725, -1.829e-16, 0.2151},
        },
        {
            {5.9761e-05, -0.37796, 1.6708e-15, 0.26725, -0.21292, 1.1736e-15},
            {5.9761e-05, 0.37796, -1.5117e-15, 0.26725, 0.21292, -1.0626e-15},
            {5.9761e-05, -7.6264e-16, -0.37796, 0.26725, -4.7231e-17, -0.21292},
            {5.9761e-05, 4.9779e-16, 0.37796, 0.26725, -1.522e-17, 0.21292},
        },
        {
            {5.9761e-05, -0.37796, 1.7388e-15, 0.26725, -0.21089, 5.8358e-16},
            {5.9761e-05, 0.37796, -1.3997e-15, 0.26725, 0.21089, -4.0362e-16},
            {5.9761e-05, -7.2739e-16, -0.37796, 0.26725, -9.3883e-17, -0.21089},
            {5.9761e-05, 7.5475e-16, 0.37796, 0.26725, 1.6083e-16, 0.21089},
        },
        {
            {5.9761e-05, -0.37796, 1.6818e-15, 0.26725, -0.20899, 3.303e-16},
            {5.9761e-05, 0.37796, -1.3164e-15, 0.26725, 0.20899, -2.0651e-16},
            {5.9761e-05, -1.4686e-15, -0.37796, 0.26725, -4.8204e-16, -0.20899},
            {5.9761e-05, 1.7709e-15, 0.37796, 0.26725, 6.2016e-16, 0.20899},
        },
        {
            {5.9761e-05, -0.37796, -9.6204e-16, 0.26725, -0.20721, -2.4714e-16},
            {5.9761e-05, 0.37796, 6.5053e-16, 0.26725, 0.20721, 1.9884e-16},
            {5.9761e-05, 1.3023e-15, -0.37796, 0.26725, 4.4583e-16, -0.20721},
            {5.9761e-05, -1.2949e-15, 0.37796, 0.26725, -4.5276e-16, 0.20721},
        },
        {
            {5.9761e-05, -0.37796, -1.027e-15, 0.26725, -0.20554, -9.1712e-16},
            {5.9761e-05, 0.37796, 1.5848e-15, 0.26725, 0.20554, 1.0774e-15},
            {5.9761e-05, 6.7225e-16, -0.37796, 0.26725, 1.0218e-16, -0.20554},
            {5.9761e-05, -5.3912e-16, 0.37796, 0.26725, -2.3787e-17, 0.20554},
        },
        {
            {5.9761e-05, -0.37796, 1.3297e-15, 0.26725, -0.20398, 4.8827e-16},
            {5.9761e-05, 0.37796, -1.2698e-15, 0.26725, 0.20398, -4.6158e-16},
            {5.9761e-05, -6.3502e-16, -0.37796, 0.26725, 1.7497e-16, -0.20398},
            {5.9761e-05, 5.5175e-16, 0.37796, 0.26725, -2.0483e-16, 0.20398},
        },
        {
            {5.9761e-05, -0.37796, 1.2863e-15, 0.26725, -0.20251, 3.0566e-16},
            {5.9761e-05, 0.37796, -1.429e-15, 0.26725, 0.20251, -2.6052e-16},
            {5.9761e-05, -1.2849e-15, -0.37796, 0.26725, -3.5844e-16, -0.20251},
            {5.9761e-05, 1.1731e-15, 0.37796, 0.26725, 2.613e-16, 0.20251},
        },
        {
            {5.9761e-05, -0.37796, 2.9568e-15, 0.26725, -0.20114, 1.7021e-15},
            {5.9761e-05, 0.37796, -3.0943e-15, 0.26725, 0.20114, -1.8922e-15},
            {5.9761e-05, -2.2691e-15, -0.37796, 0.26725, -6.2694e-16, -0.20114},
            {5.9761e-05, 2.4539e-15, 0.37796, 0.26725, 6.3171e-16, 0.20114},
        },
        {
            {5.9761e-05, -0.37796, 1.0818e-16, 0.26725, -0.19984, 8.5602e-17},
            {5.9761e-05, 0.37796, -2.2562e-16, 0.26725, 0.19984, -6.7931e-17},
            {5.9761e-05, -1.3215e-16, -0.37796, 0.26725, -2.494e-16, -0.19984},
            {5.9761e-05, 1.0252e-16, 0.37796, 0.26725, 2.9711e-16, 0.19984},
        },
        {
            {5.9761e-05, -0.37796, 5.1767e-16, 0.26725, -0.19863, 3.0372e-16},
            {5.9761e-05, 0.37796, -1.2942e-16, 0.26725, 0.19863, -1.734e-16},
            {5.9761e-05, 7.5073e-17, -0.37796, 0.26725, 2.3877e-16, -0.19863},
            {5.9761e-05, 6.5503e-17, 0.37796, 0.26725, -2.3721e-16, 0.19863},
        },
        {
            {5.9761e-05, -0.37796, 1.6941e-15, 0.26725, -0.19748, 8.8299e-16},
            {5.9761e-05, 0.37796, -1.6941e-15, 0.26725, 0.19748, -8.8999e-16},
            {5.9761e-05, -7.5269e-16, -0.37796, 0.26725, 4.002e-16, -0.19748},
            {5.9761e-05, 7.3682e-16, 0.37796, 0.26725, -4.1304e-16, 0.19748},
        },
        {
            {5.9761e-05, -0.37796, -1.7036e-15, 0.26725, -0.1964, -7.3888e-16},
            {5.9761e-05, 0.37796, 1.033e-15, 0.26725, 0.1964, 5.5975e-16},
            {5.9761e-05, 1.3664e-15, -0.37796, 0.26725, 4.7231e-16, -0.1964},
            {5.9761e-05, -1.2949e-15, 0.37796, 0.26725, -4.9822e-16, 0.1964},
        },
        {
            {5.9761e-05, -0.37796, 1.6048e-15, 0.26725, -0.19539, 3.6057e-16},
            {5.9761e-05, 0.37796, -1.3373e-15, 0.26725, 0.19539, -2.3144e-16},
            {5.9761e-05, -1.2388e-15, -0.37796, 0.26725, -1.1369e-15, -0.19539},
            {5.9761e-05, 1.6588e-15, 0.37796, 0.26725, 1.211e-15, 0.19539},
        },
        {
            {5.9761e-05, -0.37796, 2.4615e-16, 0.26725, -0.19443, 1.0013e-16},
            {5.9761e-05, 0.37796, -5.1691e-16, 0.26725, 0.19443, -9.2008e-17},
            {5.9761e-05, 2.997e-16, -0.37796, 0.26725, 2.9472e-16, -0.19443},
            {5.9761e-05, -1.9843e-16, 0.37796, 0.26725, -2.6574e-16, 0.19443},
        },
        {
            {5.9761e-05, -0.37796, 1.2374e-16, 0.26725, -0.19353, 1.1786e-15},
            {5.9761e-05, 0.37796, 3.5067e-17, 0.26725, 0.19353, -1.2364e-15},
            {5.9761e-05, -5.2621e-16, -0.37796, 0.26725, -3.9439e-16, -0.19353},
            {5.9761e-05, 4.3054e-16, 0.37796, 0.26725, 3.2958e-16, 0.19353},
        },
        {
            {5.9761e-05, -0.37796, 2.2342e-16, 0.26725, -0.19268, 6.8347e-16},
            {5.9761e-05, 0.37796, -3.5373e-16, 0.26725, 0.19268, -7.0339e-16},
            {5.9761e-05, 9.4232e-16, -0.37796, 0.26725, 6.7212e-16, -0.19268},
            {5.9761e-05, -8.6248e-16, 0.37796, 0.26725, -6.7993e-16, 0.19268},
        },
        {
            {5.9761e-05, -0.37796, 1.3739e-15, 0.26725, -0.19188, 8.7739e-16},
            {5.9761e-05, 0.37796, -1.0356e-15, 0.26725, 0.19188, -7.7793e-16},
            {5.9761e-05, -1.1664e-15, -0.37796, 0.26725, -3.7585e-16, -0.19188},
            {5.9761e-05, 1.5429e-15, 0.37796, 0.26725, 4.6757e-16, 0.19188},
        },
        {
            {5.9761e-05, -0.37796, -8.5921e-16, 0.26725, -0.19112, -1.3236e-16},
            {5.9761e-05, 0.37796, 9.914e-16, 0.26725, 0.19112, 1.1538e-16},
            {5.9761e-05, 7.6624e-16, -0.37796, 0.26725, -1.8322e-16, -0.19112},
            {5.9761e-05, -7.1524e-16, 0.37796, 0.26725, 2.0664e-16, 0.19112},
        },
        {
            {5.9761e-05, -0.37796, -2.3149e-15, 0.26725, -0.19041, -1.1175e-15},
            {5.9761e-05, 0.37796, 2.2594e-15, 0.26725, 0.19041, 1.0605e-15},
            {5.9761e-05, -5.1091e-16, -0.37796, 0.26725, -3.0997e-16, -0.19041},
            {5.9761e-05, 2.0436e-16, 0.37796, 0.26725, 3.0096e-16, 0.19041},
        },
        {
            {5.9761e-05, -0.37796, -2.7795e-15, 0.26725, -0.18973, 1.4744e-16},
            {5.9761e-05, 0.37796, 2.9058e-15, 0.26725, 0.18973, -9.9356e-17},
            {5.9761e-05, 3.0221e-15, -0.37796, 0.26725, 9.313e-16, -0.18973},
            {5.9761e-05, -3.0915e-15, 0.37796, 0.26725, -1.0065e-15, 0.18973},
        },
        {
            {5.9761e-05, -0.37796, 1.4609e-15, 0.26725, -0.18909, 1.0035e-15},
            {5.9761e-05, 0.37796, -1.4913e-15, 0.26725, 0.18909, -9.0429e-16},
            {5.9761e-05, -2.3765e-15, -0.37796, 0.26725, -7.9092e-16, -0.18909},
            {5.9761e-05, 2.2955e-15, 0.37796, 0.26725, 7.9704e-16, 0.18909},
        },
        {
            {5.9761e-05, -0.37796, -6.2434e-16, 0.26725, -0.18849, 2.0139e-16},
            {5.9761e-05, 0.37796, 8.8722e-16, 0.26725, 0.18849, -1.3118e-16},
            {5.9761e-05, 9.0976e-16, -0.37796, 0.26725, 1.8329e-16, -0.18849},
            {5.9761e-05, -8.5545e-16, 0.37796, 0.26725, -1.919e-16, 0.18849},
        },
        {
            {5.9761e-05, -0.37796, -2.0807e-15, 0.26725, -0.18791, -9.0355e-19},
            {5.9761e-05, 0.37796, 1.845e-15, 0.26725, 0.18791, 9.8697e-18},
            {5.9761e-05, 1.7355e-15, -0.37796, 0.26725, 7.4878e-16, -0.18791},
            {5.9761e-05, -2.0821e-15, 0.37796, 0.26725, -7.0715e-16, 0.18791},
        },
        {
            {5.9761e-05, -0.37796, -6.3082e-16, 0.26725, -0.18737, 1.4511e-16},
            {5.9761e-05, 0.37796, 4.8062e-16, 0.26725, 0.18737, -1.3087e-16},
            {5.9761e-05, 4.9944e-16, -0.37796, 0.26725, 1.6964e-16, -0.18737},
            {5.9761e-05, -9.2997e-16, 0.37796, 0.26725, -2.1356e-16, 0.18737},
        },
        {
            {5.9761e-05, -0.37796, -5.2238e-16, 0.26725, -0.18686, -4.9297e-17},
            {5.9761e-05, 0.37796, 1.3059e-15, 0.26725, 0.18686, 2.6485e-16},
            {5.9761e-05, 9.804e-16, -0.37796, 0.26725, 4.2413e-17, -0.18686},
            {5.9761e-05, -7.9768e-16, 0.37796, 0.26725, -1.3102e-17, 0.18686},
        }
    };

    // Include available gains
    for (int i = 0; i < NUM_GAINS; i++) {
        for (int row = 0; row < NUM_SERVOS; row++) {
            for (int col = 0; col < STATE_SPACE_DIM; col++) {
                lqr->avg_gains[i][row][col] = (float)_avg_gains[i][row][col];
                lqr->available_gains[_ravel_index_3d(i, row, col)] = (float)_avg_gains[i][row][col];
            }
        }
    }
    
    // Set the current gain
    lqr->current_gain = &lqr->available_gains[0];
}

void LQR_update_gain(LQR_controller* lqr, int velocity) {
    // Update gains based on speed
    if (MAX_VELOCITY < velocity && velocity < MIN_VELOCITY) {  // Stop controller if speed to high or low
        lqr->current_gain = &lqr->zero_gains[0];
    } else {
        lqr->current_gain_index = ((float)NUM_GAINS - 1) * (float)(velocity - MIN_VELOCITY) / (float)(MAX_VELOCITY - MIN_VELOCITY);
        lqr->current_gain = &lqr->available_gains[_ravel_index_3d((int)lqr->current_gain_index, 0, 0)];
    }
}

void LQR_perform_control(LQR_controller* lqr, orientation_data orientation, float* servo_defs[NUM_SERVOS]) {
    // Extract Euler angles and Rates
    float _orientation[STATE_SPACE_DIM] = {orientation.current_euler.roll, 
                                           orientation.current_euler.pitch, 
                                           orientation.current_euler.yaw, 
                                           orientation.current_rate_euler.roll, 
                                           orientation.current_rate_euler.pitch, 
                                           orientation.current_rate_euler.yaw};
    
    // Perform control
    for (int row = 0; row < NUM_SERVOS; row++) {
        *servo_defs[row] = 0.0f;
        for (int col = 0; col < STATE_SPACE_DIM; col++) {
            *servo_defs[row] += lqr->current_gain[_ravel_index_2d(row, col)] * _orientation[col];
        }
    }
}
