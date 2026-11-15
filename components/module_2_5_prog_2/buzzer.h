#pragma once

#include <stdint.h>

/**
 * @brief init hardware PWM LEDC timer for buzzer
 */
void buzzer_init(void);

/**
 * @brief set new frequency. If freq < min, switch of buzzer
 */
void buzzer_set_frequency(uint32_t freq_hz);