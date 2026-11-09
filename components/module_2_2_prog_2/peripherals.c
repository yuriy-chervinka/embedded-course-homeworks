#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "config.h"
#include "peripherals.h"

static bool isr_service_installed = false;

static void IRAM_ATTR btn_isr_handler(void *arg) {
    btn_t *btn = (btn_t*)arg;
    btn->detected = true;
}

void btn_init(btn_t *btn) {
    gpio_config_t btn_config = {
        .pin_bit_mask =(1ULL << btn->pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&btn_config);

    btn->detected = false;
    btn->pressed = false;
    btn->debouncer = 0x00'00'00'00;

    if (!isr_service_installed) {
        gpio_install_isr_service(0);
        isr_service_installed = true;
    }
    gpio_isr_handler_add(btn->pin, btn_isr_handler, (void *)btn);
}

void btn_debounce(btn_t *btn) {
    if (btn->detected) {
        int level = !gpio_get_level(btn->pin);
        btn->debouncer = (btn->debouncer << 1) | level;    // shift and set next bit to '1'

        if (btn->debouncer == 0xFF'FF'FF'FF) {              // if all bits are '1' - button is pressed
            btn->detected = false;
            btn->pressed = true;
            btn->debouncer = 0x00'00'00'00;
        } else if (btn->debouncer == 0x00'00'00'00) {       // if button released (random signal set 'detected' flag)
            btn->detected = false;
        }
    }
}

void pwm_init(pwm_t *pwm) {
    gpio_config_t led_config = {
        .pin_bit_mask =(1ULL << pwm->pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&led_config);
    gpio_set_level(pwm->pin, 0);

    pwm->setpoint = 50;     // starting at 50% brightness
    pwm->tracker = 0;
}

uint8_t pwm_update(pwm_t *pwm, btn_t *brighter, btn_t *darker) {
    if (brighter->pressed) {
        brighter->pressed = false;
        if (pwm->setpoint < 100) {
            pwm->setpoint += PWM_STEP;
            return pwm->setpoint;
        } else {
            return 150;     // Max level reached
        }
    }
    if (darker->pressed) {
        darker->pressed = false;
        if (pwm->setpoint > 0) {
            pwm->setpoint -= PWM_STEP;
            return pwm->setpoint;
        } else {
            return 200;     // Min level reached
        }
    }
    return 250;             // no button was pressed
}

void pwm_handler(pwm_t *pwm) {
    if (pwm->tracker < 100) {
        pwm->tracker++;
    } else pwm->tracker = 0;
    
    if ((pwm->tracker <= pwm->setpoint) && (pwm->setpoint > 0)) {
        gpio_set_level(pwm->pin, 1);
    } else {
        gpio_set_level(pwm->pin, 0);
    }
}