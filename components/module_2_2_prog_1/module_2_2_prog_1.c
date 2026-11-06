#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "config.h"
#include "peripherals.h"

static const char *TAG = "MOD_2_2_PROG_1";

typedef struct {
    uint64_t sum;
    uint32_t counter;
    uint32_t min;
    uint32_t max;
    uint32_t ave;
} delay_t;

static void process_delay(delay_t *delay_stats);

void run_module_2_2_prog_1() {
    // initiallizing peripherals
    in_init(IN_PIN);
    out_init(OUT_PIN);

    delay_t ON_delay = { .min = 0xFFFFFFFF, .max = 0 };
    delay_t OFF_delay = { .min = 0xFFFFFFFF, .max = 0 };

    ESP_LOGI(TAG, "System is running");

    while (1) {
        uint8_t state = toggle_output(OUT_PIN, IN_PIN);
        
        uint32_t ticks_passed = 0;
        bool is_hardware_ok = false;

        // Internal loop guards from relay malfunction. 
        // If no input signal is received within 100ms, a new cycle begins, avoiding an infinite wait.
        while (ticks_passed < SAFETY_TIMEOUT_TICKS) {
            if (input_detected()) {
                is_hardware_ok = true;
                break;
            }
            vTaskDelay(1);
            ticks_passed++;
        }

        if (is_hardware_ok) {
            if (state == 0) {
                process_delay(&OFF_delay);
                ESP_LOGI(TAG, "Relay CLOSED (OFF) delay in us:\t Count: %u | Min: %u | Max: %u | Ave: %u", OFF_delay.counter, OFF_delay.min, OFF_delay.max, OFF_delay.ave);
            }
            else {
                process_delay(&ON_delay);
                ESP_LOGI(TAG, "Relay OPENED (ON) delay in us: \t Count: %u | Min: %u | Max: %u | Ave: %u", ON_delay.counter, ON_delay.min, ON_delay.max, ON_delay.ave);
            }
        } else {
            ESP_LOGE(TAG, "Timeout! Relay missed closing/opening signal within 100ms. Skipping delay processing.");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void process_delay(delay_t *delay_stats) {
    uint32_t delay = get_delay_us();
    delay_stats->counter++;
    delay_stats->sum += delay;
    delay_stats->ave = delay_stats->sum / delay_stats->counter;
    if (delay_stats->max < delay) delay_stats->max = delay;
    if (delay_stats->min > delay) delay_stats->min = delay;
}