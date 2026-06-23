#include "Lilygot547Battery.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lilygo_t5_47_battery {

static const char *const TAG = "lilygo_t5_47_battery";

void Lilygot547Battery::setup() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
}

void Lilygot547Battery::update() {
  epd_poweron();
  delay(100);
  this->update_battery_voltage_();
  epd_poweroff();
}

void Lilygot547Battery::update_battery_voltage_() {
  int raw = adc1_get_raw(ADC1_CHANNEL_0);
  if (raw < 0) {
    ESP_LOGW(TAG, "Failed to read ADC");
    return;
  }

  // Ongecalibreerde conversie: 12-bit ADC, spanningsdeler x2, vaste
  // Vref-aanname van 1100mV (typische ESP32 default zonder eFuse-calibratie).
  // Minder nauwkeurig dan curve-fitting calibratie, maar voldoende voor
  // een indicatieve batterijspanning.
  const float vref_mv = 1100.0f;
  float battery_voltage = ((float) raw / 4095.0f) * 2.0f * 3.3f * (vref_mv / 1000.0f);

  if (this->voltage != nullptr) {
    this->voltage->publish_state(battery_voltage);
  }

  ESP_LOGD(TAG, "Batterij ADC raw=%d, spanning=%.2fV", raw, battery_voltage);
}

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
