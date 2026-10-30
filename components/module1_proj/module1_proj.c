#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include "adc.h"
#include "filter.h"

#define OUT             GPIO_NUM_6
#define TRESHHOLD_DARK  2500
#define TRESHHOLD_LIGHT 3500

static const char *TAG = "MODULE1_PROJ";

static filter_sma_t adc_buffer_filter;
uint16_t adc = 2500;
uint8_t out = 0;

void run_module1_proj() {
    // initiallizing ADC
    if (adc_init() != ESP_OK) {
        ESP_LOGE(TAG, "Critical error: ADC initialization failed!");
        return;
    }
    filter_sma_init(&adc_buffer_filter);
    
    // configure out pin
    gpio_config_t out_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << OUT),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&out_config);


    ESP_LOGI(TAG, "System is running.");

    while (1) {
        adc = adc_read_raw();
        adc = filter_sma_update(&adc_buffer_filter, adc);
        if (adc < TRESHHOLD_DARK) {
            out = 1;
        }
        if (adc > TRESHHOLD_LIGHT) {
            out = 0;
        }
        gpio_set_level(OUT, out);

        ESP_LOGI(TAG, "Lighting level: %lu  | Output state: %lu", adc, out);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}