#pragma once

#include <stdint.h>

#ifdef __INTELLISENSE__
  #ifndef constexpr
    #define constexpr const
  #endif
#endif

static constexpr uint8_t  LED =           4;
static constexpr uint8_t  BTN_BRIGHTER =  6;
static constexpr uint8_t  BTN_DARKER =    0;
static constexpr uint8_t  PWM_STEP =      10;   // duty cycle changing step in %
