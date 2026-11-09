#pragma once

#include <stdint.h>

#ifdef __INTELLISENSE__
  #ifndef constexpr
    #define constexpr const
  #endif
#endif

static constexpr uint8_t  OUT_PIN = 4;
static constexpr uint8_t  IN_PIN = 6;
static constexpr uint32_t DEBOUNCE_WINDOW_US = 100'000;  // 100 miliseconds
static constexpr uint32_t  SAFETY_TIMEOUT_TICKS = pdMS_TO_TICKS(100);  // 100 miliseconds in ticks