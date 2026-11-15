#include <driver/gpio.h>
#include <esp_timer.h>
#include "config.h"
#include"encoder.h"

static volatile int32_t encoder_pos = 0;
static volatile bool position_updated = false;
static uint8_t last_clk_state = 1;

// timer callback (called every 1 ms)
static void encoder_timer_callback(void *arg) {
    uint8_t current_clk = gpio_get_level(ENCODER_CLK);

    if (last_clk_state == 1 && current_clk == 0) {      // detecting `Falling` state change of CLK (1 -> 0) 
        uint8_t dt_state = gpio_get_level(ENCODER_DT);

        if (dt_state != current_clk) {
            encoder_pos++;      // clockwise rotation
        } else {
            encoder_pos--;      // counterclockwise rotation
        }
        position_updated = true;
    }
    last_clk_state = current_clk;
}

void encoder_init(void) {
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << ENCODER_CLK) | (1ULL << ENCODER_DT),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_config);

    last_clk_state = gpio_get_level(ENCODER_CLK);

    const esp_timer_create_args_t timer_args = {
        .callback = &encoder_timer_callback,
        .name = "encoder_poll_timer"
    };

    esp_timer_handle_t poll_timer;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &poll_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(poll_timer, ENCODER_POLL_US));
}

int32_t encoder_get_position(void) {
    position_updated = false;
    return encoder_pos;
}

bool encoder_is_updated(void) {
    return position_updated;
}