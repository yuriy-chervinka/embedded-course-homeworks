#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include "config.h"
#include "btn.h"

static volatile bool s_btn_pressed = false;
static uint32_t s_last_debounce_timestamp_ms = 0;

static void IRAM_ATTR btn_isr_handler(void *arg) {
    uint32_t current_timestamp_ms = (uint32_t)(esp_timer_get_time() / 1000);

    if ((current_timestamp_ms - s_last_debounce_timestamp_ms) > DEBOUNCE_DELAY_MS) {
        s_btn_pressed = true;
    }
    s_last_debounce_timestamp_ms = current_timestamp_ms;
}

void btn_init(uint8_t pin) {
    gpio_config_t io_config = {
        .pin_bit_mask =(1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&io_config);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin, btn_isr_handler, (void *)(uintptr_t)pin);
}

void btn_handler(led_config_t *led_config) {
    if (s_btn_pressed) {
        led_config->mode++;
        if (led_config->mode >= MODE_COUNT) {
            led_config->mode = OFF;
        }
        s_btn_pressed = false;
    }
}