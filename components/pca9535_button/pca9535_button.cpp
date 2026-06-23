#include "pca9535_button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pca9535_button {

static const char *const TAG = "pca9535_button";

static const uint8_t REG_INPUT_PORT1 = 0x01;
static const uint8_t REG_CONFIG_PORT1 = 0x07;
static const uint8_t BUTTON_BIT = 2;

void PCA9535Button::setup() {
  ESP_LOGCONFIG(TAG, "PCA9535 knop-uitlezer opgezet op adres 0x%02X", this->address_);

  // Lees de huidige Configuration Port 1 waarde, zodat we alle andere
  // pinnen (die de e-paper voeding aansturen) ongewijzigd laten.
  uint8_t config_val = 0;
  if (this->read_byte(REG_CONFIG_PORT1, &config_val)) {
    ESP_LOGD(TAG, "Config Port1 voor wijziging: 0x%02X", config_val);

    // Zet ENKEL bit 2 op 1 (input), laat alle andere bits exact zoals ze waren.
    uint8_t new_config = config_val | (1 << BUTTON_BIT);

    if (new_config != config_val) {
      this->write_byte(REG_CONFIG_PORT1, new_config);
      ESP_LOGD(TAG, "Config Port1 na wijziging: 0x%02X (bit %d op input gezet)", new_config, BUTTON_BIT);
    } else {
      ESP_LOGD(TAG, "Bit %d stond al op input, geen wijziging nodig", BUTTON_BIT);
    }
  } else {
    ESP_LOGW(TAG, "Kon Config Port1 niet lezen - knop-pin NIET geconfigureerd, wees voorzichtig");
  }
}

void PCA9535Button::update() {
  uint8_t reg_val = 0;
  if (!this->read_byte(REG_INPUT_PORT1, &reg_val)) {
    ESP_LOGW(TAG, "Kon Input Port 1 niet lezen");
    return;
  }

  bool pressed = !((reg_val >> BUTTON_BIT) & 0x01);

  if (this->binary_sensor_ != nullptr) {
    this->binary_sensor_->publish_state(pressed);
  }

  ESP_LOGD(TAG, "Input Port1 raw=0x%02X, bit%d=%d, knop ingedrukt=%s",
           reg_val, BUTTON_BIT, (reg_val >> BUTTON_BIT) & 1, pressed ? "JA" : "NEE");
}

}  // namespace pca9535_button
}  // namespace esphome
