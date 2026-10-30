#pragma once

#include <stdint.h>
#include <esp_err.h>

#define ADC_PIN_NUM         4
#define ADC_UNIT_ID         ADC_UNIT_1
#define ADC_CHANNEL_ID      ADC_CHANNEL_3

#define ADC_MAX_VOLTAGE_MV  3100

/**
 * @brief One-time ADC configuration and chip callibration
 * @return
 *     - ESP_OK success
 *     - ESP_ERR error
*/
esp_err_t adc_init(void);

/**
 * @brief Raw analog data read (0...4095)
 * @return Raw ADC value
 */
uint32_t adc_read_raw(void) __attribute__((warn_unused_result));

/**
 * @brief Reding the exact voltage in milivolts taking eFuse calibration into account
 * @return Voltage in milivolts (0...3000 mV)
 */
uint32_t adc_read_millivolts(void) __attribute__((warn_unused_result));