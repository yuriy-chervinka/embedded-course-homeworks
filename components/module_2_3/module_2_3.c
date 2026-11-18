#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "config.h"
#include "peripherals.h"

static const char *TAG = "MOD_2_3";
static bool blinking_on = true;
static bool blinking_speed_default = true;

void run_module_2_3() {
    // initiallizing peripherals
    btn_t btn_stop = { .pin = BTN_STOP };
    btn_init(&btn_stop);
    
    btn_t btn_speed_change = { .pin = BTN_SPEED_CHANGE };
    btn_init(&btn_speed_change);
    
    led_t led_red = { .pin = LED_RED_PIN, .half_period = LED_RED_SPEED };
    led_init(&led_red);
    
    led_t led_green = { .pin = LED_GREEN_PIN, .half_period = LED_GREEN_SPEED };
    led_init(&led_green);
    
    led_t led_blue = { .pin = LED_BLUE_PIN, .half_period = LED_BLUE_DEFAULT_SPEED };
    led_init(&led_blue);
    
    led_t *leds[3] = {&led_red, &led_green, &led_blue};
    int64_t last_debounce = esp_timer_get_time();
    int64_t last_wdt_reset = esp_timer_get_time();
    
    ESP_LOGI(TAG, "System is running");

    while (1) {
        int64_t now = esp_timer_get_time();

        // blinking LEDs if not paused
        if (blinking_on) {
            for (uint8_t i=0; i < 3; i++) {
                if((now - leds[i]->last_change) >= leds[i]->half_period) {
                    leds[i]->last_change = now;
                    gpio_set_level(leds[i]->pin, !gpio_get_level(leds[i]->pin));
                }
            }
        }

        // buttons debouncing
        if (now - last_debounce > 500) {
            last_debounce = now;
            btn_debounce(&btn_stop);
            btn_debounce(&btn_speed_change);
        }

        // handle pause button
        if (btn_stop.pressed) {
            btn_stop.pressed = false;
            blinking_on = !blinking_on;
        }

        // handle speed change button
        if (btn_speed_change.pressed) {
            btn_speed_change.pressed = false;
            blinking_speed_default = !blinking_speed_default;
            led_blue.half_period = blinking_speed_default ? LED_BLUE_DEFAULT_SPEED : LED_BLUE_UPDATED_SPEED;
        }

        // Watchdog reset every 1 sec
        if (now - last_wdt_reset > 1'000'000) {
            last_wdt_reset = now;
            vTaskDelay(1);
        }
    }
}