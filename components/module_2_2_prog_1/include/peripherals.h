#pragma once

#include <stdint.h>

/**
 * @brief Input initializer
 * @param pin   GPIO_NUM to initialize
*/
void in_init(uint8_t pin);

/**
 * @brief Output initializer
 * @param pin   GPIO_NUM to initialize
*/
void out_init(uint8_t pin);

/**
 * @brief Get and clear status of `input_detected` flag
 * @note Called from main loop
 * @return bool flag
 */
bool input_detected();

/**
 * @brief Get delay in microseconds between output signel selt and input signal received
 * @return uint32_t delay in microseconds
 */
uint32_t get_delay_us(void);

/**
 * @brief Toggle ouput state
 * @param out_pin   GPIO_NUM of output to toggle
 * @param in_pin    GPIO_NUM of input which detects input change
 * @return uint8_t state of GPIO
 */
uint8_t toggle_output(uint8_t out_pin, uint8_t in_pin);