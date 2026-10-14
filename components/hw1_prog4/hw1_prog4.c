#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <soc/io_mux_reg.h>
#include <soc/gpio_reg.h>
#include <soc/gpio_periph.h>
#include <driver/gptimer.h>
#include <esp_rom_gpio.h>
#include <esp_log.h>

#define RED_LED 4

static const char *TAG = "HW1_PROG4";

static volatile uint32_t * const gpio_out_w1ts DRAM_ATTR = (volatile uint32_t *)(DR_REG_GPIO_BASE + 0x0008);
static volatile uint32_t * const gpio_out_w1tc DRAM_ATTR = (volatile uint32_t *)(DR_REG_GPIO_BASE + 0x000C);
static volatile uint32_t * const gpio_enable_w1ts       = (volatile uint32_t *)(DR_REG_GPIO_BASE + 0x0024);

static volatile bool led_state = false;
static volatile uint32_t current_delay_ms = 1600;
static volatile bool delay_decreasing = true;

static uint32_t delay_min_ms = 200;
static uint32_t delay_max_ms = 1600;
static uint32_t delay_step_ms = 100;

static bool IRAM_ATTR on_timer_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    uint32_t pin = *(uint32_t *)user_ctx;

    led_state = !led_state;
    if (led_state) {
        *gpio_out_w1ts = (1ULL << pin);
    } else {
        *gpio_out_w1tc = (1ULL << pin);
    }

    if (!led_state) {
        if (delay_decreasing) {
            if (current_delay_ms > delay_min_ms) {
                current_delay_ms -= delay_step_ms;
            } else {
                delay_decreasing = false;
                current_delay_ms += delay_step_ms;
            }
        } else {
            if (current_delay_ms < delay_max_ms) {
                current_delay_ms += delay_step_ms;
            } else {
                delay_decreasing = true;
                current_delay_ms -= delay_step_ms;
            }
        }

        gptimer_alarm_config_t next_alarm_config = {
            .alarm_count = current_delay_ms * 1000,
            .flags.auto_reload_on_alarm = true,
            .reload_count = 0,
        };
        gptimer_set_alarm_action(timer, &next_alarm_config);
    }
    return true;
}

static inline void led_pin_init(uint32_t pin) {
    esp_rom_gpio_pad_select_gpio(pin);

    uint32_t io_mux_reg_addr = GPIO_PIN_MUX_REG[pin];
    REG_CLR_BIT(io_mux_reg_addr, FUN_PU);
    REG_CLR_BIT(io_mux_reg_addr, FUN_PD);
    
    volatile uint32_t *gpio_pin_config = (volatile uint32_t *)(DR_REG_GPIO_BASE + 0x0074 + (pin * 4));
    
    *gpio_enable_w1ts = (1ULL << pin);
    *gpio_pin_config = 0;
    *gpio_out_w1tc = (1ULL << pin);

}

void run_hw1_prog4() {
    static uint32_t target_pin = RED_LED;

    // configure pin
    led_pin_init(RED_LED);

    // configure timer
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,
    };
    gptimer_new_timer(&timer_config, &gptimer);

    gptimer_event_callbacks_t cbs = { .on_alarm = on_timer_alarm_cb };
    gptimer_register_event_callbacks(gptimer, &cbs, &target_pin);

    gptimer_alarm_config_t alarm_config = {
        .alarm_count = current_delay_ms * 1000,
        .flags.auto_reload_on_alarm = true,
        .reload_count = 0,
    };
    gptimer_set_alarm_action(gptimer, &alarm_config);

    gptimer_enable(gptimer);
    gptimer_start(gptimer);

    ESP_LOGI(TAG, "All set. Entering the loop.");

    // blink loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}