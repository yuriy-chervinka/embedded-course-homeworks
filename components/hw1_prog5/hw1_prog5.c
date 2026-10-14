#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define LED GPIO_NUM_4

static const char *TAG = "HW1_PROG5";


void run_hw1_prog5() {
    // configure pins
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    // blink loop
    while (1) {
        gpio_set_level(LED, 1);
        ESP_LOGI(TAG, "Red LED OFF Blue LED ON");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED, 0);
        ESP_LOGI(TAG, "Red LED ON Blue LED OFF");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}