#include "bq25896_battery.h"
#include "esphome/core/log.h"

namespace esphome {
namespace bq25896_battery {

static const char *const TAG = "bq25896_battery";

// BQ2589x registeradressen
static const uint8_t REG0B_SYS_STATUS = 0x0B;
static const uint8_t REG0E_BATV = 0x0E;

void BQ25896Battery::setup() {
  ESP_LOGCONFIG(TAG, "BQ25896 batterijmonitor opgezet op adres 0x%02X", this->address_);
}

void BQ25896Battery::update() {
  float voltage = this->read_battery_voltage_();
  if (voltage <= 0.0f) {
    ESP_LOGW(TAG, "Kon batterijspanning niet lezen");
    return;
  }

  if (this->voltage_sensor_ != nullptr) {
    this->voltage_sensor_->publish_state(voltage);
  }

  if (this->percentage_sensor_ != nullptr) {
    float pct = this->voltage_to_percentage_(voltage);
    this->percentage_sensor_->publish_state(pct);
  }

  if (this->status_sensor_ != nullptr) {
    std::string status = this->read_charge_status_();
    this->status_sensor_->publish_state(status);
  }

  ESP_LOGD(TAG, "Batterijspanning: %.2fV", voltage);
}

float BQ25896Battery::read_battery_voltage_() {
  uint8_t reg_val = 0;
  if (!this->read_byte(REG0E_BATV, &reg_val)) {
    return -1.0f;
  }

  // Bit 7 = THERM_STAT (negeren), bits 6-0 = BATV
  uint8_t batv_code = reg_val & 0x7F;

  // VBAT = 2304mV offset + batv_code * 20mV/stap
  float voltage_mv = 2304.0f + (batv_code * 20.0f);
  return voltage_mv / 1000.0f;
}

float BQ25896Battery::voltage_to_percentage_(float voltage) {
  // Eenvoudige lineaire LiPo-ontlaadcurve-benadering (3.0V = 0%, 4.2V = 100%)
  // Niet perfect lineair in werkelijkheid, maar een redelijke benadering
  // zonder volledige curve-tabel.
  const float v_min = 3.0f;
  const float v_max = 4.2f;

  float pct = ((voltage - v_min) / (v_max - v_min)) * 100.0f;
  if (pct > 100.0f) pct = 100.0f;
  if (pct < 0.0f) pct = 0.0f;
  return pct;
}

std::string BQ25896Battery::read_charge_status_() {
  uint8_t reg_val = 0;
  if (!this->read_byte(REG0B_SYS_STATUS, &reg_val)) {
    return "Onbekend";
  }

  // Bits 4-3 = CHRG_STAT
  uint8_t chrg_stat = (reg_val >> 3) & 0x03;
  // Bit 2 = PG_STAT (power good / VBUS aanwezig)
  bool power_good = (reg_val >> 2) & 0x01;

  if (!power_good) {
    return "Ontladen (geen voeding)";
  }

  switch (chrg_stat) {
    case 0:
      return "Niet aan het laden";
    case 1:
      return "Pre-charge";
    case 2:
      return "Snel laden";
    case 3:
      return "Volledig opgeladen";
    default:
      return "Onbekend";
  }
}

}  // namespace bq25896_battery
}  // namespace esphome
