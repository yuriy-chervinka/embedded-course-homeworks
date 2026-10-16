#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <esp_timer.h>

#define RED_LED GPIO_NUM_4
#define BLUE_LED GPIO_NUM_5
#define BTN_EXT GPIO_NUM_6
#define BTN_INT GPIO_NUM_0

static const char *TAG = "HW2_PROG1";

typedef enum {
    MODE_1,
    MODE_2
} system_mode_t;

static TaskHandle_t manager_task_handle = NULL;

static void execute_mode_1() {
    ESP_LOGD(TAG, "Mode 1 step");
    static uint8_t led_state = 0;

    gpio_set_level(RED_LED, led_state);
    gpio_set_level(BLUE_LED, led_state);
    led_state = !led_state;

    vTaskDelay(pdMS_TO_TICKS(200));
}

static void execute_mode_2() {
    ESP_LOGI(TAG, "Mode 2 step");
    static uint8_t led_state = 0;

    gpio_set_level(RED_LED, led_state);
    led_state = !led_state;
    gpio_set_level(BLUE_LED, led_state);

    vTaskDelay(pdMS_TO_TICKS(1000));
}

static void IRAM_ATTR btn_isr_handler(void *arg) {
    uint32_t gpio_num = (uint32_t)arg;

    // debounce
    static int64_t last_time = 0;
    int64_t now = esp_timer_get_time();
    if (now - last_time < 50000) { return; }
    last_time = now;

    // message to manager task
    xTaskNotifyFromISR(manager_task_handle, gpio_num, eSetValueWithOverwrite, NULL);
}

static void button_manager_task(void *pvParameters) {
    uint32_t gpio_num = 0;

    // set default mode
    system_mode_t current_mode = MODE_1;

    while (1) {
        // check notification
        BaseType_t notify_result = xTaskNotifyWait(0x00, ULONG_MAX, &gpio_num, 0);

        // if notification - change machine state
        if (notify_result == pdPASS) {
            if (gpio_num == BTN_EXT) {
                current_mode = MODE_1;
                ESP_LOGI(TAG, "Event: Switched to Mode 1");
            }
            else if (gpio_num == BTN_INT) {
                current_mode = MODE_2;
                ESP_LOGI(TAG, "Event: Switched to Mode 2");
            }
        }

        // run task depending on state
        if (current_mode == MODE_1) {
            execute_mode_1();
        } else if (current_mode == MODE_2) {
            execute_mode_2();
        }
    }
}

void run_hw2_prog1() {
    // configure pins
    gpio_config_t led_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << RED_LED) | (1ULL << BLUE_LED),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config_t btn_config = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BTN_EXT) | (1ULL << BTN_INT),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    gpio_config(&led_config);
    gpio_config(&btn_config);

    // create task manager
    xTaskCreate(button_manager_task, "btn_manager", 2048, NULL, 5, &manager_task_handle);

    // isr init
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BTN_EXT, btn_isr_handler, (void *)BTN_EXT);
    gpio_isr_handler_add(BTN_INT, btn_isr_handler, (void *)BTN_INT);

    ESP_LOGI(TAG, "System initialized");
}