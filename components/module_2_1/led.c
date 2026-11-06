#include <driver/gpio.h>
#include <esp_timer.h>
#include "led.h"

void led_init(led_t *self, led_config_t *config) {
    if (self == NULL || config == NULL) return;

    self->config = config;

    gpio_config_t io_config = {
        .pin_bit_mask =(1ULL << config->pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_config);

    self->last_change_timestamp_ms = (uint32_t)(esp_timer_get_time() / 1000);
    self->state = (config->mode == MODE_ON) ? ON : OFF;
    gpio_set_level(config->pin, self->state);
}

void led_handler(led_t *self) {
    if (self == NULL) return;

    switch (self->config->mode) {
        case MODE_OFF:
            if (self->state != OFF) {
                self->state = OFF;
                gpio_set_level(self->config->pin, self->state);
            }
            break;

        case MODE_ON:
            if (self->state != ON) {
                self->state = ON;
                gpio_set_level(self->config->pin, self->state);
            }
            break;

        case MODE_BLINK:
            uint32_t current_timestamp_ms = (uint32_t)(esp_timer_get_time() / 1000);
            if (current_timestamp_ms - self->last_change_timestamp_ms >= self->config->blink_half_period_ms) {
                self->state = !self->state;
                gpio_set_level(self->config->pin, self->state);
                self->last_change_timestamp_ms = current_timestamp_ms;
            }
            break;

        case MODE_COUNT:
            break;
    }
}