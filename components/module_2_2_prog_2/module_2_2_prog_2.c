#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "config.h"
#include "peripherals.h"

static const char *TAG = "MOD_2_2_PROG_2";

void run_module_2_2_prog_2() {
    // initiallizing peripherals
    btn_t btn_brighter = { .pin = BTN_BRIGHTER };
    btn_init(&btn_brighter);
    
    btn_t btn_darker = { .pin = BTN_DARKER };
    btn_init(&btn_darker);
    
    pwm_t led = { .pin = LED };
    pwm_init(&led);
    
    int64_t last_wdt_reset = esp_timer_get_time();
    int64_t last_debounce = esp_timer_get_time();
    
    ESP_LOGI(TAG, "System is running");

    while (1) {
        pwm_handler(&led);
        int64_t now = esp_timer_get_time();

        if (now - last_debounce > 500) {
            last_debounce = now;
            btn_debounce(&btn_brighter);
            btn_debounce(&btn_darker);
    
            uint8_t update_state = pwm_update(&led, &btn_brighter, &btn_darker);
            pwm_handler(&led);
            
            if (update_state != 250) {
                switch (update_state) {
                    case 150: ESP_LOGI(TAG, "Max brightness level reached"); break;
                    case 200: ESP_LOGI(TAG, "Min brightness level reached"); break;
                    default: ESP_LOGI(TAG, "Brightness updated. New setpoint is %d %%", update_state); break;
                }
            }
        }

        // Watchdog reset every 1 sec
        if (now - last_wdt_reset > 1'000'000) {
            last_wdt_reset = now;
            vTaskDelay(1);
        }
    }
}