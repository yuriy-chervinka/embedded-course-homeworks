#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * Button controller
 * @param pin       GPIO_NUM to read
 * @param detected  isr trigger button (initial action)
 * @param pressed   button pressed (set after debouce filtered)
 * @param debouncer debouncer counter
 */ 
typedef struct {
    uint8_t pin;
    bool detected;
    bool pressed;
    uint32_t debouncer;
} btn_t;    

/**
 * @brief Button initializer
 * @param btn   pointer to button
*/
void btn_init(btn_t *btn);

/**
 * @brief debounce a pressed button
 * @param btn   pointer to button
 */
void btn_debounce(btn_t *btn);

/**
 * @brief LED controller
 * @param pin           LED GPIO_NUM
 * @param half_period   blinking half period
 * @param last_change   last state change timestamp
 */  
typedef struct {
    uint8_t pin;
    uint32_t half_period;
    int64_t last_change;
} led_t;        

/**
 * @brief LED initializer
 * @param led   pointer to LED
*/  
void led_init(led_t *led);