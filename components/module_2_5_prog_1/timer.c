#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gptimer.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include "config.h"
#include "timer.h"

static const char *TAG = "MOD_2_5_PROG_1";

static gptimer_handle_t gptimer = NULL;
static TaskHandle_t logger_task_handle = NULL;
static gptimer_alarm_config_t alarm_cfg = {
    .alarm_count = TICKS_ACTIVE_END,
    .reload_count = 0,
    .flags.auto_reload_on_alarm = false,
};

static volatile bool is_fun_on = true;

static inline void IRAM_ATTR update_gpios(bool state) {
    gpio_set_level(RELAY, state ? 1 : 0);
    gpio_set_level(LED_ACTIVE, state ? 1 : 0);
    gpio_set_level(LED_IDLE, state ? 0 : 1);
}

// hardware timer ISR 
static bool IRAM_ATTR on_timer_alarm(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    BaseType_t high_task_awoken = pdFALSE;

    if (is_fun_on) {     // active phase ended. Switch OFF relay, timer count till the cycle end
        is_fun_on = false;
        update_gpios(is_fun_on);

        alarm_cfg.alarm_count = TICKS_CYCLE_END;
        alarm_cfg.flags.auto_reload_on_alarm = true;   // reload counter at the next interrupt

    } else {            // cycle ended. Switch ON relay, timer count till the end of active phase
        is_fun_on = true;
        update_gpios(is_fun_on);

        alarm_cfg.alarm_count = TICKS_ACTIVE_END;
        alarm_cfg.flags.auto_reload_on_alarm = false;  // don't reload counter at the next interrupt
    }

    gptimer_set_alarm_action(timer, &alarm_cfg);

    if (logger_task_handle != NULL) {
        vTaskNotifyGiveFromISR(logger_task_handle, &high_task_awoken);
    }

    return (high_task_awoken == pdTRUE);
}

// RTOS background logging task
static void timer_logger_task(void *arg) {
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    // sleep untill notified from ISR

        if (is_fun_on) {
            ESP_LOGI(TAG, "Cycle restarted. Relay: ON");
        } else {
            ESP_LOGI(TAG, "Setpoint reached. Relay: OFF");
        }
    }
}


void timer_start(void) {
    // configure pins
    gpio_config_t io_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << RELAY) | (1ULL << LED_ACTIVE) | (1ULL << LED_IDLE),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&io_config);

    // create logger task
    xTaskCreate(timer_logger_task, "timer_logger_task", 2048, NULL, 5, &logger_task_handle);

    // timer init
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = TIMER_RES_HZ,
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    // binding ISR
    gptimer_event_callbacks_t cbs = { .on_alarm = on_timer_alarm, };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

    // initial state
    is_fun_on = true;
    update_gpios(is_fun_on);
    ESP_LOGI(TAG, "System initialized.");

    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_cfg));

    // starting timer
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}