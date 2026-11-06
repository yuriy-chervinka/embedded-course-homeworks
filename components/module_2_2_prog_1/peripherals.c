#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include "config.h"
#include "peripherals.h"

static volatile bool s_input_detected = false;
static volatile uint64_t s_in_timestamp_us = 0;
static uint64_t s_out_timestamp_us = 0;
static uint64_t s_debounce_timestamp_us = 0;
static uint8_t s_out_state = 0;

static void IRAM_ATTR in_isr_handler(void *arg) {
    uint64_t now = esp_timer_get_time();

    if ((now - s_debounce_timestamp_us) > DEBOUNCE_WINDOW_US) {
        s_in_timestamp_us = now;
        s_input_detected = true;
        s_debounce_timestamp_us = now;
    }
}

void in_init(uint8_t pin) {
    gpio_config_t in_config = {
        .pin_bit_mask =(1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };
    gpio_config(&in_config);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin, in_isr_handler, (void *)(uintptr_t)pin);

    s_input_detected = false;
}

void out_init(uint8_t pin) {
    gpio_config_t out_config = {
        .pin_bit_mask =(1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&out_config);
    gpio_set_level(pin, s_out_state);
}

bool input_detected() {
    if (s_input_detected) {
        s_input_detected = false;
        return true;
    }
    return false;
}

uint32_t get_delay_us(void) {
    if (s_in_timestamp_us >= s_out_timestamp_us) {
        return (uint32_t)(s_in_timestamp_us - s_out_timestamp_us);
    }
    return 0;
}

uint8_t toggle_output(uint8_t out_pin, uint8_t in_pin) {
    s_out_state = !s_out_state;

    s_in_timestamp_us = 0;
    s_input_detected = false;
    s_debounce_timestamp_us = 0;
    
    gpio_set_level(out_pin, s_out_state);
    s_out_timestamp_us = esp_timer_get_time();
    
    return s_out_state;
}