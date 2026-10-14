#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hal/gpio_ll.h>
#include <soc/gpio_struct.h>
#include <esp_rom_gpio.h>
#include <esp_rom_sys.h>
#include <esp_log.h>

#define RED_LED 4
#define BLUE_LED 5
#define GREEN_LED 6

static const char *TAG = "HW1_PROG3";

static inline void led_pin_init(uint32_t pin) {
    esp_rom_gpio_pad_select_gpio(pin);
    gpio_ll_output_enable(&GPIO, pin);
    gpio_ll_od_disable(&GPIO, pin);
    gpio_ll_pullup_dis(&GPIO, pin);
    gpio_ll_pulldown_dis(&GPIO, pin);
    gpio_ll_intr_disable(&GPIO, pin);
}

void run_hw1_prog3() {
    // configure pins
    led_pin_init(RED_LED);
    led_pin_init(BLUE_LED);
    led_pin_init(GREEN_LED);

    // blink loop
    while (1) {
        gpio_ll_set_level(&GPIO, RED_LED, 1);
        vTaskDelay(pdMS_TO_TICKS(200));
        gpio_ll_set_level(&GPIO, BLUE_LED, 1);
        vTaskDelay(pdMS_TO_TICKS(200));
        gpio_ll_set_level(&GPIO, GREEN_LED, 1);
        vTaskDelay(pdMS_TO_TICKS(800));
        ESP_LOGI(TAG, "All LEDs ON");

        gpio_ll_set_level(&GPIO, RED_LED, 0);
        vTaskDelay(pdMS_TO_TICKS(200));
        gpio_ll_set_level(&GPIO, BLUE_LED, 0);
        vTaskDelay(pdMS_TO_TICKS(200));
        gpio_ll_set_level(&GPIO, GREEN_LED, 0);
        vTaskDelay(pdMS_TO_TICKS(1200));
        ESP_LOGI(TAG, "All LEDs OFF");
    }
}