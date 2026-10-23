#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "bsp_adc.h"
#include "filter.h"

static const char *TAG = "HW4_PROG1";

static filter_sma_t adc_buffer_filter;

void run_hw4_prog1() {
    // initiallizing ADC
    if (bsp_adc_init() != ESP_OK) {
        ESP_LOGE(TAG, "Critical error: ADC initialization failed!");
        return;
    }
    filter_sma_init(&adc_buffer_filter);

    ESP_LOGI(TAG, "ADC monitorring system is running.");

    while (1) {
        uint32_t raw = bsp_adc_read_raw();
        uint32_t filtered_raw = filter_sma_update(&adc_buffer_filter, raw);
        uint32_t calc = (raw * BSP_ADC_MAX_VOLTAGE_MV) / 4095;
        uint32_t mv = bsp_adc_read_millivolts();
        uint32_t error = (calc > mv) ? (calc - mv) : (mv - calc);
        ESP_LOGI(TAG, "Raw Input: %lu  |  Filtered Raw Input: %lu  |  Calculated: %lu mV  |  Actual: %lu mV  |  Error: %4lu mV", raw, filtered_raw, calc, mv, error);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}