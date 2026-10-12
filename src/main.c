#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define RED_LED 48
#define BLUE_LED 2

static const char *TAG = "ESP32-led-blink";


void app_main() {
    // configure pins
    gpio_reset_pin(RED_LED);
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);
    gpio_reset_pin(BLUE_LED);
    gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT);

    // blink loop
    while (1) {
        gpio_set_level(RED_LED, 1);
        ESP_LOGI(TAG, "Red LED ON");
        gpio_set_level(BLUE_LED, 0);
        ESP_LOGI(TAG, "Blue LED OFF");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        gpio_set_level(RED_LED, 0);
        ESP_LOGI(TAG, "Red LED OFF");
        gpio_set_level(BLUE_LED, 1);
        ESP_LOGI(TAG, "Blue LED ON");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}