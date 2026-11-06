#pragma once

#include <stdint.h>

#ifdef __INTELLISENSE__
  #ifndef constexpr
    #define constexpr const
  #endif
#endif

static constexpr uint8_t  LED_PIN = 4;
static constexpr uint32_t LED_BLINK_PERIOD_MS = 1000;

static constexpr uint8_t  BTN_PIN = 6;
static constexpr uint32_t DEBOUNCE_DELAY_MS = 50;