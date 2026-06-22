#include "Lilygot547Battery.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lilygo_t5_47_battery {

static const char *const TAG = "lilygo_t5_47_battery";

// ADC1_CHANNEL_0 op ESP32-S3 = GPIO1
static const adc_channel_t BATTERY_ADC_CHANNEL = ADC_CHANNEL_0;
static const adc_atten_t BATTERY_ADC_ATTEN = ADC_ATTEN_DB_11;

void Lilygot547Battery::setup() {
  // ADC1 unit aanmaken
  adc_oneshot_unit_init_cfg_t init_config = {
      .unit_id = ADC_UNIT_1,
  };
  esp_err_t err = adc_oneshot_new_unit(&init_config, &this->adc_handle_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to create ADC unit: %d", err);
    return;
  }

  // Kanaal configureren
  adc_oneshot_chan_cfg_t chan_config = {
      .atten = BATTERY_ADC_ATTEN,
      .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  err = adc_oneshot_config_channel(this->adc_handle_, BATTERY_ADC_CHANNEL, &chan_config);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to configure ADC channel: %d", err);
    return;
  }

  this->calibrate_adc_();
}

void Lilygot547Battery::calibrate_adc_() {
  adc_cali_curve_fitting_config_t cali_config = {
      .unit_id = ADC_UNIT_1,
      .chan = BATTERY_ADC_CHANNEL,
      .atten = BATTERY_ADC_ATTEN,
      .bitwidth = ADC_BITWIDTH_DEFAULT,
  };
  esp_err_t err = adc_cali_create_scheme_curve_fitting(&cali_config, &this->cali_handle_);
  if (err == ESP_OK) {
    this->calibrated_ = true;
    ESP_LOGI(TAG, "ADC calibration (curve fitting) succeeded");
  } else {
    this->calibrated_ = false;
    ESP_LOGW(TAG, "ADC calibration failed (%d), using raw conversion fallback", err);
  }
}

void Lilygot547Battery::update() {
  epd_poweron();
  delay(100);
  this->update_battery_voltage_();
  epd_poweroff();
}

void Lilygot547Battery::update_battery_voltage_() {
  if (this->adc_handle_ == nullptr) {
    ESP_LOGW(TAG, "ADC not initialized");
    return;
  }

  int raw = 0;
  esp_err_t err = adc_oneshot_read(this->adc_handle_, BATTERY_ADC_CHANNEL, &raw);
  if (err != ESP_OK) {
    ESP_LOGW(TAG, "Failed to read ADC: %d", err);
    return;
  }

  float battery_voltage;

  if (this->calibrated_) {
    int voltage_mv = 0;
    err = adc_cali_raw_to_voltage(this->cali_handle_, raw, &voltage_mv);
    if (err == ESP_OK) {
      // x2 voor spanningsdeler
      battery_voltage = (voltage_mv / 1000.0f) * 2.0f;
    } else {
      ESP_LOGW(TAG, "Calibrated conversion failed (%d), falling back to raw", err);
      battery_voltage = ((float) raw / 4095.0f) * 2.0f * 3.3f;
    }
  } else {
    // Fallback zonder calibratie
    battery_voltage = ((float) raw / 4095.0f) * 2.0f * 3.3f;
  }

  if (this->voltage != nullptr) {
    this->voltage->publish_state(battery_voltage);
  }

  ESP_LOGD(TAG, "Battery raw=%d, voltage=%.2f V", raw, battery_voltage);
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
