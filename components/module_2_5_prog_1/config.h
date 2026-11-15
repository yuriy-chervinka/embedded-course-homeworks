#pragma once

#include <stdint.h>
#include <driver/gpio.h>

#ifdef __INTELLISENSE__
  #ifndef constexpr
    #define constexpr const
  #endif
#endif

static constexpr gpio_num_t RELAY =             GPIO_NUM_4;
static constexpr gpio_num_t LED_ACTIVE =        GPIO_NUM_5;
static constexpr gpio_num_t LED_IDLE =          GPIO_NUM_6;

static constexpr uint64_t TIMER_RES_HZ =        10'000;                   // 1 tick = 0.1 ms

#define TEST_MODE   1

#if TEST_MODE
  static constexpr uint64_t TICKS_ACTIVE_END =  (3 * TIMER_RES_HZ);       // 3 sec
  static constexpr uint64_t TICKS_CYCLE_END =   (12 * TIMER_RES_HZ);      // 12 sec
#else
  static constexpr uint64_t TICKS_ACTIVE_END =  (15 * 60 * TIMER_RES_HZ); // 15 min
  static constexpr uint64_t TICKS_CYCLE_END =   (60 * 60 * TIMER_RES_HZ); // 60 min
#endif