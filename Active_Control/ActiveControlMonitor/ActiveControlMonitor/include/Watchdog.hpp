#include <Arduino.h>
#include "ValueDefinitions.hpp"

#define WDT_PIN 2 // in ms

class Watchdog
{
public:
    void start()
    {
        NRF_WDT->CONFIG = 0x01;
        NRF_WDT->CRV = (uint32_t)((WDT_TIMEOUT * 32.768) + 1);
        NRF_WDT->RREN = 0x01;
        NRF_WDT->TASKS_START = 1;
    };

    void feed()
    {
        NRF_WDT->RR[0] = WDT_RR_RR_Reload;
    };
};