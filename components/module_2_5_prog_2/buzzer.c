#include <driver/ledc.h>
#include "config.h"
#include "buzzer.h"

void buzzer_init(void) {
    ledc_timer_config_t ledc_timer = {
        .speed_mode         = LEDC_LOW_SPEED_MODE,
        .timer_num          = LEDC_TIMER_0,
        .duty_resolution    = LEDC_TIMER_10_BIT,
        .freq_hz            = 440UL,                // 440 Hz, A-tone of 4th octave
        .clk_cfg            = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .speed_mode         = LEDC_LOW_SPEED_MODE,
        .channel            = LEDC_CHANNEL_0,
        .timer_sel          = LEDC_TIMER_0,
        .intr_type          = LEDC_INTR_DISABLE,
        .gpio_num           = BUZZER,
        .duty               = 0,                    // buzzer off at start
        .hpoint             = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void buzzer_set_frequency(uint32_t freq_hz) {
    if (freq_hz == 0) {
        // switch off PWM (duty 0%)
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        return;
    }

    // update timer frequency
    ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, freq_hz);

    // set duty 50%
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 512);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}