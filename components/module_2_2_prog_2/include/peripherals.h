#pragma once

#include <stdint.h>

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
 * @brief PWM controller
 * @param pin   GPIO_NUM controlled
 * @param setpoint     duty cycle 0...100%
 * @param tracker      cycle tracker
 */  
typedef struct {
    uint8_t pin;
    uint8_t setpoint;
    uint8_t tracker;
} pwm_t;        

/**
 * @brief PWM initializer
 * @param pwm   pointer to PWM
*/  
void pwm_init(pwm_t *pwm);

/**
 * @brief updates PWM by reading buttons state
 * @param pwm       pointer to pwm
 * @param brighter  pointer to button which increases brightness
 * @param darker    pointer to button which decreases brightness
 * @return          new duty cycle setpoint if change is valid, 150 if max level reached, 200 if min level reached, 250 if no updates triggered
 */
uint8_t pwm_update(pwm_t *pwm, btn_t *brighter, btn_t *darker);

/**
 * @brief PWM handler
 * @param pwm   pointer to pwm
 */
void pwm_handler(pwm_t *pwm);