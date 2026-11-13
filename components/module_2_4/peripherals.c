#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "config.h"
#include "peripherals.h"

static bool isr_service_installed = false;
static volatile bool btn_press_detected = false;
static volatile uint32_t raw_counter = 0;            // task 1 counter
static volatile uint32_t time_based_counter = 0;      // task 2 counter
static uint32_t state_based_counter = 0;     // task 3 counter
static uint32_t fsm_counter = 0;            // task 4 counter

static void IRAM_ATTR btn_isr_handler(void *arg) {
    // task 1
    raw_counter++;

    // task 2
    btn_tb_t *btn = (btn_tb_t*)arg;
    if ((btn->current_timestamp_us - btn->last_timestamp_us) > btn->debounce_delay_us) {
        time_based_counter++;
        btn->last_timestamp_us = btn->current_timestamp_us;
    }

    // task 3
    btn_press_detected = true;
}

void isr_init(gpio_num_t pin, void *arg) {
    if (!isr_service_installed) {
        gpio_install_isr_service(0);
        isr_service_installed = true;
    }
    gpio_isr_handler_add(pin, btn_isr_handler, (void *)arg);

}

void btn_init(gpio_num_t pin) {
    gpio_config_t btn_config = {
        .pin_bit_mask =(1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&btn_config);
}

uint32_t raw_counter_getter(void) {      // task 1
    return raw_counter;
}

uint32_t time_based_counter_getter(void) {     // task 2
    return time_based_counter;
}

uint32_t state_based_debouncing(int current_level) {  // task 3
    if (btn_press_detected) {
        if (current_level == BTN_PRESSED) {
            state_based_counter++;
            btn_press_detected = false;
        }
    }
    return state_based_counter;
}

uint32_t polling_fsm(btn_sb_t *btn) {     // task 4
    switch (btn->state) {
        case IDLE_:
            if (btn->current_level == BTN_PRESSED) {   // button pressed, register state changing
                btn->last_timestamp_us = btn->current_timestamp_us;
                btn->state = CHECK_PRESS_;
            }
            break;

        case CHECK_PRESS_:
            if (btn->current_level == BTN_RELEASED) {  // button released - bouncing
                btn->state = IDLE_;
                break;
            }
            if ((btn->current_timestamp_us - btn->last_timestamp_us) > btn->debounce_delay_us) { // stable signal, register button pressed
                btn->state = PRESSED;
                fsm_counter++;
                break;
            }
            break;
        
        case PRESSED:
            if (btn->current_level == BTN_RELEASED) {  // button released, register state changing
                btn->last_timestamp_us = btn->current_timestamp_us;
                btn->state = CHECK_RELEASE_;
            }
            break;

        case CHECK_RELEASE_:
            if (btn->current_level == BTN_PRESSED) {  // button pressed - bouncing
                btn->state = PRESSED;
                break;
            }
            if ((btn->current_timestamp_us - btn->last_timestamp_us) > btn->debounce_delay_us) { // stable signal, register button released
                btn->state = IDLE_;
                break;
            }
            break;
    }
    return fsm_counter;
}

void extended_fsm(btn_ext_sb_t *btn) {   // additional task
    switch (btn->state) {
        case IDLE:
            if (btn->current_level == BTN_PRESSED) {   // button pressed, register state changing
                btn->last_timestamp_us = btn->current_timestamp_us;
                btn->state = CHECK_PRESS;
            }
            break;

        case CHECK_PRESS:
            if (btn->current_level == BTN_RELEASED) {  // button released - bouncing
                btn->state = IDLE;
                break;
            }
            if ((btn->current_timestamp_us - btn->last_timestamp_us) > btn->debounce_delay_us) { // stable for short time, register short press
                btn->state = SHORT_PRESSED;

                if (!btn->trigger_on_release) {  // event triggered on button press (after debouncing)
                    btn->event = SHORT_PRESS;
                }
                break;
            }
            break;
        
        case SHORT_PRESSED:
            if (btn->current_level == BTN_RELEASED) {  // button released, register state changing
                btn->last_timestamp_us = btn->current_timestamp_us;
                btn->state = CHECK_RELEASE_AFTER_SHORT;

                if (btn->trigger_on_release) {  // event triggered on button release
                    btn->event = SHORT_PRESS;
                }
                break;
            }
            if ((btn->current_timestamp_us - btn->last_timestamp_us) > btn->long_press_delay_us) {  // stable for long time, register long press
                btn->state = LONG_PRESSED;

                if (!btn->trigger_on_release) {  // event triggered on button press
                    btn->event = LONG_PRESS;
                }
                break;
            }
            break;

        case LONG_PRESSED:
            if (btn->current_level == BTN_RELEASED) {  // button released, register state changing
                btn->last_timestamp_us = btn->current_timestamp_us;
                btn->state = CHECK_RELEASE_AFTER_LONG;

                if (btn->trigger_on_release) {  // event triggered on button release
                    btn->event = LONG_PRESS;
                }
                break;
            }
            break;

        case CHECK_RELEASE_AFTER_SHORT:
            if (btn->current_level == BTN_PRESSED) {  // button pressed - bouncing
                btn->state = SHORT_PRESSED;
                break;
            }
            if ((btn->current_timestamp_us - btn->last_timestamp_us) > btn->debounce_delay_us) { // stable signal, register button released
                btn->state = IDLE;
                break;
            }
            break;

        case CHECK_RELEASE_AFTER_LONG:
            if (btn->current_level == BTN_PRESSED) {  // button pressed - bouncing
                btn->state = LONG_PRESSED;
                break;
            }
            if ((btn->current_timestamp_us - btn->last_timestamp_us) > btn->debounce_delay_us) { // stable signal, register button released
                btn->state = IDLE;
                break;
            }
            break;
    }
}

void led_init(led_t *led) {
    gpio_config_t led_config = {
        .pin_bit_mask =(1ULL << led->pin),
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&led_config);
    gpio_set_level(led->pin, 0);
}