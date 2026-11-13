#pragma once

#include <stdint.h>
#include <driver/gpio.h>

/**
 * @brief Time-based button controller
 * @param last_timestamp_us     latest level change time for debouncing
 * @param current_timestamp_us  current time for debouncingbtn
 * @param debounce_delay_us     debouncing delay setpoint
 */ 
typedef struct {
    int64_t last_timestamp_us;
    int64_t current_timestamp_us;
    int64_t debounce_delay_us;
} btn_tb_t;    

/**
 * @brief button states for FSM
 */
typedef enum : uint8_t {
    IDLE_,
    CHECK_PRESS_,
    PRESSED,
    CHECK_RELEASE_,
} btn_state_t;

/**
 * @brief State-based button controller
 * @param state                 FSM
 * @param last_timestamp_us     latest level change time for debouncing
 * @param current_timestamp_us  current time for debouncingbtn
 * @param debounce_delay_us     debouncing delay setpoint
 * @param current_level         0 if btn pressed, 1 if released
 */ 
typedef struct {
    btn_state_t state;
    int64_t last_timestamp_us;
    int64_t current_timestamp_us;
    int64_t debounce_delay_us;
    int current_level;
} btn_sb_t;

/**
 * @brief button extended states for FSM
 */
typedef enum : uint8_t {
    IDLE,
    CHECK_PRESS,
    SHORT_PRESSED,
    LONG_PRESSED,
    CHECK_RELEASE_AFTER_SHORT,
    CHECK_RELEASE_AFTER_LONG,
} btn_ext_state_t;

/**
 * @brief button extended states for FSM
 */
typedef enum : uint8_t {
    NO_EVENTS,
    SHORT_PRESS,
    LONG_PRESS,
} btn_events_t;

/**
 * @brief Extended state-based button controller
 * @param trigger_on_release    event appears on button release if true. if false - event appears on button press
 * @param state                 FSM
 * @param event                 event registered
 * @param last_timestamp_us     latest level change time for debouncing
 * @param current_timestamp_us  current time for debouncingbtn
 * @param debounce_delay_us     debouncing delay setpoint
 * @param long_press_delay_us   debouncing delay setpoint
 * @param current_level         0 if btn pressed, 1 if released
 */ 
typedef struct {
    bool trigger_on_release;
    btn_ext_state_t state;
    btn_events_t event;
    int64_t last_timestamp_us;
    int64_t current_timestamp_us;
    int64_t debounce_delay_us;
    int64_t long_press_delay_us;
    int current_level;
} btn_ext_sb_t;    

/**
 * @brief raw button press
 * @return raw interrupts count
 */
uint32_t raw_counter_getter(void);

/**
 * @brief time-based debounced button press
 * @return time-based filtered button press count
 */
uint32_t time_based_counter_getter(void);

/**
 * @brief state-based debounced button press
 * @param current_level current pin state
 * @return state-based filtered button press count
 */
uint32_t state_based_debouncing(int current_level);

/**
 * @brief FSM based button state
 * @param btn   pointer to button
 */
uint32_t polling_fsm(btn_sb_t *btn);

/**
 * @brief Extended FSM based button state
 * @param btn   pointer to button
 */
void extended_fsm(btn_ext_sb_t *btn);

/**
 * @brief Button initializer
 * @param pin   button GPIO_NUM
*/
void btn_init(gpio_num_t pin);

/**
 * @brief isr handlers types for isr_init
 */
typedef enum : uint8_t {
    MAIN_ISR,
    A_ISR,
} isr_handlers_t;

/**
 * @brief isr handlers initializer
 * @param pin   GPIO_NUM to link
 * @param isr_handler   isr_handler to add
 * @param arg   parameter for isr handler
 */
void isr_init(gpio_num_t pin, void *arg);

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