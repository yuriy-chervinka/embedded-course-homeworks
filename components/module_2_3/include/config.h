#pragma once

#include <stdint.h>

#ifdef __INTELLISENSE__
  #ifndef constexpr
    #define constexpr const
  #endif
#endif

static constexpr uint8_t  BTN_STOP =                0;
static constexpr uint8_t  BTN_SPEED_CHANGE =        15;
static constexpr uint8_t  LED_RED_PIN =             4;
static constexpr uint32_t  LED_RED_SPEED =          100'000;
static constexpr uint8_t  LED_GREEN_PIN =           5;
static constexpr uint32_t  LED_GREEN_SPEED =        250'000;
static constexpr uint8_t  LED_BLUE_PIN =            6;
static constexpr uint32_t  LED_BLUE_DEFAULT_SPEED = 500'000;
static constexpr uint32_t  LED_BLUE_UPDATED_SPEED = 50'000;