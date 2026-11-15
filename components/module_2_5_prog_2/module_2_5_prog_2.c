#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "config.h"
#include "encoder.h"
#include "buzzer.h"

static const char *TAG = "MOD_2_5_PROG_2";

void run_module_2_5_prog_2() {
    encoder_init();
    buzzer_init();

    int32_t current_freq = 440;
    int32_t last_pos = 0;

    buzzer_set_frequency(current_freq);
    ESP_LOGI(TAG, "System started.");

    while (1) {
        if (encoder_is_updated()) {
            int32_t new_pos = encoder_get_position();
            int32_t delta = new_pos - last_pos;
            last_pos = new_pos;

            // calculating new frequency
            current_freq = (delta < 0) ? (int32_t)(current_freq / 2) : (int32_t)(current_freq * 2);
            if (current_freq > BUZZER_MAX_FREQ) {
                current_freq = BUZZER_MAX_FREQ;
            }
            if (current_freq < BUZZER_MIN_FREQ) {
                current_freq = BUZZER_MIN_FREQ;
            }

            buzzer_set_frequency(current_freq);
            ESP_LOGI(TAG, "Encoder pos: %ld | Buzzer freq: %ld Hz", new_pos, current_freq);
        }

        vTaskDelay(1);      // let Watchdog to reset itself
    }
}
