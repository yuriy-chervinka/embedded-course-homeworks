#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "config.h"
#include "peripherals.h"

static const char *TAG = "MOD_2_4";

static btn_tb_t btn_1 = { .debounce_delay_us = BTN_DEBOUNCE_DELAY_US };
static btn_sb_t btn_fsm = { .debounce_delay_us = BTN_DEBOUNCE_DELAY_US };
static btn_ext_sb_t btn_2 = { 
    .trigger_on_release = true, 
    .debounce_delay_us = BTN_DEBOUNCE_DELAY_US,
    .long_press_delay_us = BTN_LONG_PRESS_DELAY_US
  };
static led_t led = { .pin = LED, .half_period = LED_SPEED_1 };

void run_module_2_4() {
    // initiallizing peripherals
    btn_init(BTN_1);
    btn_init(BTN_2);
    isr_init(BTN_1, &btn_1);
    led_init(&led);

    int64_t last_wdt_reset_us = esp_timer_get_time();

    ESP_LOGI(TAG, "System is running");

    while (1) {
        int64_t current_timestamp_us = esp_timer_get_time();
        int btn_1_level = gpio_get_level(BTN_1);
        int btn_2_level = gpio_get_level(BTN_2);
        btn_1.current_timestamp_us = current_timestamp_us;

        uint32_t raw = raw_counter_getter();                        // task 1
        uint32_t time_based = time_based_counter_getter();          // task 2
        uint32_t state_based = state_based_debouncing(btn_1_level); // task 3

        btn_fsm.current_timestamp_us = current_timestamp_us;
        btn_fsm.current_level = btn_1_level;
        uint32_t polling = polling_fsm(&btn_fsm);                   // task 4
        
        btn_2.current_timestamp_us = current_timestamp_us;
        btn_2.current_level = btn_2_level;
        extended_fsm(&btn_2);                                       // additional task

        // blinking LED
        if (btn_2.event == SHORT_PRESS) {
            btn_2.event = NO_EVENTS;
            led.half_period = LED_SPEED_1;
        } else if (btn_2.event == LONG_PRESS) {
            btn_2.event = NO_EVENTS;
            led.half_period = LED_SPEED_2;
        }
        if ((current_timestamp_us - led.last_change) >= led.half_period) {
            led.last_change = current_timestamp_us;
            gpio_set_level(led.pin, !gpio_get_level(led.pin));
        }

        // Watchdog reset every 1 sec
        if (current_timestamp_us - last_wdt_reset_us > 1'000'000) {
            last_wdt_reset_us = current_timestamp_us;
            vTaskDelay(1);

            // print results every 1 sec
            ESP_LOGI(TAG, "raw: %d | time-based: %d | state-based: %d | polling: %d", raw, time_based, state_based, polling);
        }
    }
}