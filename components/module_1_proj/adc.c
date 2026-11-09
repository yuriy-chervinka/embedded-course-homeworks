#include "adc.h"
#include <esp_log.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>

static const char *TAG = "ADC";

// Global hardware descriptors
static adc_oneshot_unit_handle_t    s_adc_handle = NULL;
static adc_cali_handle_t            s_cali_handle = NULL;
static bool                         s_is_calibrated = false;

// Internal service function for applying factory calibration
static esp_err_t local_adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle) {
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;

    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = unit,
        .chan = channel,
        .atten = atten,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);

    if (ret == ESP_OK) {
        *out_handle = handle;
    }
    return ret;
}

esp_err_t adc_init(void) {
    // 1. Create new ADC unit
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_ID,
    };
    esp_err_t err = adc_oneshot_new_unit(&init_config, &s_adc_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create ADC unit handle: %s", esp_err_to_name(err));
        return err;
    }

    // 2. Configure the selected channel (12 bit, 12 dB attenuation for measurement up to 3.3V)
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };
    err = adc_oneshot_config_channel(s_adc_handle, ADC_CHANNEL_ID, &config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure ADC channel: %s", esp_err_to_name(err));
        return err;
    }

    // 3. Initialize the factory eFuse calibration for chip linearity
    err = local_adc_calibration_init(ADC_UNIT_ID, ADC_CHANNEL_ID, ADC_ATTEN_DB_12, &s_cali_handle);
    if (err == ESP_OK) {
        s_is_calibrated = true;
        ESP_LOGI(TAG, "Hardware ADC calibration successfull.");
    } else {
        ESP_LOGE(TAG, "Factory ADC calibration not available. Reading will be less precise.");
    }
    return ESP_OK;
}

uint32_t adc_read_raw(void) {
    int raw_val = 0;
    if (s_adc_handle != NULL) {
        esp_err_t err = adc_oneshot_read(s_adc_handle, ADC_CHANNEL_ID, &raw_val);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Error reading raw ADC value: %s", esp_err_to_name(err));
            return 0;
        }
    }
    return (uint32_t)raw_val;
}

uint32_t adc_read_millivolts(void) {
    uint32_t raw = adc_read_raw();
    int voltage_mv = 0;

    if (s_is_calibrated && s_cali_handle != NULL) {
        // Converting ADC units to actual voltage using the chip's eFuse coefficient map
        adc_cali_raw_to_voltage(s_cali_handle, raw, &voltage_mv);
        return (uint32_t)voltage_mv;
    } else {
        // Default math calculation if callibration is missing
        return (raw * ADC_MAX_VOLTAGE_MV) / 4095;
    }
}