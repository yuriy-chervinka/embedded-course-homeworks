#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include "bsp_board.h"

static const char *TAG = "HW3_PROG1";

static volatile uint32_t DRAM_ATTR isr_counter = 0;

static void IRAM_ATTR btn_isr_counter(void *arg) {
    isr_counter++;
}

void run_hw3_prog1() {
    // configure pin
    gpio_config_t btn_config = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BSP_BUTTON_MAIN),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    gpio_config(&btn_config);

    // isr init
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BSP_BUTTON_MAIN, btn_isr_counter, NULL);
    ESP_LOGI(TAG, "System initialized");

    uint32_t loop_counter = 0;
    while (1) {
        if (loop_counter != isr_counter) {
            loop_counter = isr_counter;
            ESP_LOGI(TAG, "Button pressed: %lu times", loop_counter);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}