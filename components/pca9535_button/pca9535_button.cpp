#include "pca9535_button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pca9535_button {

static const char *const TAG = "pca9535_button";

// PCA9535 Input Port 1 register, bit 2 = IO1.2 (de rand-knop)
static const uint8_t REG_INPUT_PORT1 = 0x01;
static const uint8_t BUTTON_BIT = 2;

void PCA9535Button::setup() {
  ESP_LOGCONFIG(TAG, "PCA9535 knop-uitlezer opgezet op adres 0x%02X", this->address_);
}

void PCA9535Button::update() {
  uint8_t reg_val = 0;
  if (!this->read_byte(REG_INPUT_PORT1, &reg_val)) {
    ESP_LOGW(TAG, "Kon Input Port 1 niet lezen");
    return;
  }

  // De knop is typisch actief-laag (pull-up, ingedrukt = 0).
  // Als de knop active-high blijkt te zijn, verwijder de '!' hieronder.
  bool pressed = !((reg_val >> BUTTON_BIT) & 0x01);

  this->publish_state(pressed);

  ESP_LOGD(TAG, "Input Port 1 raw=0x%02X, knop ingedrukt=%s", reg_val, pressed ? "JA" : "NEE");
}

}  // namespace pca9535_button
}  // namespace esphome
