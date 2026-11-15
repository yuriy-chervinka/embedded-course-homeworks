#pragma once

#include <stdint.h>
#include <driver/gpio.h>

#ifdef __INTELLISENSE__
  #ifndef constexpr
    #define constexpr const
  #endif
#endif

static constexpr gpio_num_t ENCODER_CLK =   GPIO_NUM_4;
static constexpr gpio_num_t ENCODER_DT =    GPIO_NUM_5;
static constexpr gpio_num_t BUZZER =        GPIO_NUM_6;

static constexpr uint64_t ENCODER_POLL_US = 1'000;      // 1 ms

static constexpr uint32_t BUZZER_MIN_FREQ = 55;         // A-tone of 1st octave
static constexpr uint32_t BUZZER_MAX_FREQ = 7040;       // A-tone of 8th octave