#pragma once

#include <stdint.h>
#include <driver/gpio.h>

#ifdef __INTELLISENSE__
  #ifndef constexpr
    #define constexpr const
  #endif
#endif

static constexpr int        BTN_PRESSED =             0;
static constexpr int        BTN_RELEASED =            1;

static constexpr uint32_t   BTN_DEBOUNCE_DELAY_US =   50'000;
static constexpr uint32_t   BTN_LONG_PRESS_DELAY_US = 1'000'000;

static constexpr gpio_num_t BTN_1 =                   GPIO_NUM_5;
static constexpr gpio_num_t BTN_2 =                   GPIO_NUM_6;

static constexpr gpio_num_t LED =                     GPIO_NUM_4;
static constexpr uint32_t   LED_SPEED_1 =             100'000;
static constexpr uint32_t   LED_SPEED_2 =             300'000;