#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "config.h"
#include "led.h"
#include "btn.h"

static const char *TAG = "MODULE_2_1";

void run_module_2_1() {
    // configure led
    led_config_t led_config = {
        .pin = LED_PIN,
        .mode = MODE_BLINK,
        .blink_half_period_ms = LED_BLINK_PERIOD_MS / 2
    };

    // init periferals
    led_t led;
    led_init(&led, &led_config);
    btn_init(BTN_PIN);

    // blink loop
    uint16_t loop_counter = 0;
    uint64_t loop_timestamp_us = esp_timer_get_time();
    while (1) {
        loop_counter +=1;

        // minimum delay passes control to RTOS scheduler and clear Watchdog timer
        uint64_t vtaskdelay_timestamp_us = esp_timer_get_time();
        vTaskDelay(1);

        if (loop_counter >= 1000) {
            uint64_t current_timestamp_us = esp_timer_get_time();
            ESP_LOGI(TAG, "\nAverage duration of 1 cycle including 'vTaskDelay(1)': %llu us", (current_timestamp_us - loop_timestamp_us) / 1'000);
            ESP_LOGI(TAG, "Duration of 1 'vTaskDelay(1)': %llu us", current_timestamp_us - vtaskdelay_timestamp_us);
            ESP_LOGI(TAG, "Duration of cycle without 'vTaskDelay(1)': %llu us", ((current_timestamp_us - loop_timestamp_us) / 1'000) - (current_timestamp_us - vtaskdelay_timestamp_us));
            loop_timestamp_us = current_timestamp_us;
            loop_counter = 0;
        }

        led_handler(&led);
        btn_handler(&led_config);
    }
}