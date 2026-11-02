#pragma once

#include <stdint.h>
#include "led.h"

/**
 * @brief Button initializer
 * @param pin   GPIO_NUM to which button is connected
*/
void btn_init(uint8_t pin);

/**
 * @brief Handle button press logic
 * @param config    Pointer to LED configuration struct
 * @note Called from main loop
 */
void btn_handler(led_config_t *led_config);