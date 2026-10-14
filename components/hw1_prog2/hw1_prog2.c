#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define RED_LED GPIO_NUM_1
#define BLUE_LED GPIO_NUM_2

static const char *TAG = "HW1_PROG2";

typedef struct {
    gpio_num_t pin;
    uint32_t delay_ms;
    const char *led_name;
} led_config_t;

static led_config_t red_led = { .pin = RED_LED, .delay_ms = 700, .led_name = "RED"};
static led_config_t blue_led = { .pin = BLUE_LED, .delay_ms = 1100, .led_name = "BLUE"};

void led_blink(void *pvParameters) {
    led_config_t *config = (led_config_t *)pvParameters;
    uint8_t led_state = 0;

    while (1) {
        gpio_set_level(config->pin, led_state);
        ESP_LOGI(TAG, "%s LED %d", config->led_name, led_state);
        led_state = !led_state;
        vTaskDelay(pdMS_TO_TICKS(config->delay_ms));
    }
}

void run_hw1_prog2() {
    // configure pins
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << RED_LED) | (1ULL << BLUE_LED),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&io_config);

    // blink tasks
    xTaskCreate(led_blink, "red_led", 2048, &red_led, 5, NULL);
    xTaskCreate(led_blink, "blue_led", 2048, &blue_led, 5, NULL);
}