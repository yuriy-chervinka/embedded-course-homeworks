#pragma once

#include <stdint.h>

#define FILTER_CIRCULAR_BUFFER_SIZE     16
#define FILTER_CIRCULAR_WINDOW_MASK     (FILTER_CIRCULAR_BUFFER_SIZE - 1)

/**
 * @brief Simple Moving Average (SMA) filter structure
 */
typedef struct {
    uint32_t buffer[FILTER_CIRCULAR_BUFFER_SIZE];
    uint32_t running_sum;
    uint16_t buffer_idx;
} filter_sma_t;

/**
 * @brief SMA filter initializer
 * @param self pointer to filter instance
 */
void filter_sma_init(filter_sma_t *self);

/**
 * @brief Updates SMA filter with new value and returns result
 * @param self pointer to filter instance
 * @param new_sample new raw value
 * @return uint32_t filtered value (moving average of the window)
 */
uint32_t filter_sma_update(filter_sma_t *self, uint32_t new_sample);