#pragma once

#include <stdint.h>

/**
 * @brief LED allowed states
 */
typedef enum : uint8_t {
    OFF,
    ON,
} led_state_t;

/**
 * @brief LED working modes
 */
typedef enum : uint8_t {
    MODE_OFF,
    MODE_ON,
    MODE_BLINK,
    MODE_COUNT,
} led_mode_t;

/**
 * @brief LED configuration struct
 * @param pin                   GPIO_NUM which controlling the LED
 * @param mode                  LED mode
 * @param blink_half_period_ms  Half period of blinking cycle in milliseconds
 */
typedef struct {
    uint8_t     pin;
    led_mode_t  mode;
    uint32_t    blink_half_period_ms;
} led_config_t;

/**
 * @brief LED instance struct
 * @param config                    Pointer to LED configure struct
 * @param state                     LED state
 * @param last_change_timestamp_ms  Timestamp of last state change in milliseconds
 */
typedef struct {
    led_config_t    *config;
    led_state_t     state;
    uint32_t        last_change_timestamp_ms;
} led_t;

/**
 * @brief LED initializer
 * @param self      Pointer to LED instance struct
 * @param config    Pointer to LED configuration struct
*/
void led_init(led_t *self, led_config_t *config);

/**
 * @brief Handle LED state depending on config and current time
 * @param self  Pointer to LED instance
 */
void led_handler(led_t *self);