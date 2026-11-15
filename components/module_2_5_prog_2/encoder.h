#pragma once

#include <stdint.h>

/**
 * @brief init encoder and start polling timer
 */
void encoder_init(void);

/**
 * @brief returns encoder position
 */
int32_t encoder_get_position(void);

/**
 * @brief check if encoder position is updated
 */
bool encoder_is_updated(void);