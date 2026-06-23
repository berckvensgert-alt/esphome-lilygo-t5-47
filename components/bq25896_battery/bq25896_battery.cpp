#include "bq25896_battery.h"
#include "esphome/core/log.h"

namespace esphome {
namespace bq25896_battery {

static const char *const TAG = "bq25896_battery";

static const uint8_t REG02_ADC_CTRL = 0x02;
static const uint8_t REG0B_SYS_STATUS = 0x0B;
static const uint8_t REG0E_BATV = 0x0E;

void BQ25896Battery::setup() {
  ESP_LOGCONFIG(TAG, "BQ25896 batterijmonitor opgezet op adres 0x%02X", this->address_);

  // Continue ADC-conversie activeren (CONV_RATE=1) en een eerste
  // conversie starten (CONV_START=1), zonder andere bits in REG02
  // te verstoren.
  uint8_t reg02_val = 0;
  if (this->read_byte(REG02_ADC_CTRL, &reg02_val)) {
    reg02_val |= 0xC0;  // bit7 CONV_START=1, bit6 CONV_RATE=1
    this->write_byte(REG02_ADC_CTRL, reg02_val);
    ESP_LOGD(TAG, "ADC-conversie geactiveerd (REG02=0x%02X)", reg02_val);
  } else {
    ESP_LOGW(TAG, "Kon REG02 niet lezen om ADC te activeren");
  }
}

void BQ25896Battery::update() {
  // Elke update een nieuwe conversie triggeren (CONV_START is
  // self-clearing na conversie, dus opnieuw zetten als CONV_RATE
  // uitstaat of als extra zekerheid).
  uint8_t reg02_val = 0;
  if (this->read_byte(REG02_ADC_CTRL, &reg02_val)) {
    reg02_val |= 0x80;  // CONV_START
    this->write_byte(REG02_ADC_CTRL, reg02_val);
  }

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

  uint8_t batv_code = reg_val & 0x7F;
  float voltage_mv = 2304.0f + (batv_code * 20.0f);
  return voltage_mv / 1000.0f;
}

float BQ25896Battery::voltage_to_percentage_(float voltage) {
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

  uint8_t chrg_stat = (reg_val >> 3) & 0x03;
  bool power_good = (reg_val >> 2) & 0x01;

  if (!power_good) {
    return "Ontladen (geen voeding)";
  }

  switch (chrg_stat) {
    case 0: return "Niet aan het laden";
    case 1: return "Pre-charge";
    case 2: return "Snel laden";
    case 3: return "Volledig opgeladen";
    default: return "Onbekend";
  }
}

}  // namespace bq25896_battery
}  // namespace esphome
