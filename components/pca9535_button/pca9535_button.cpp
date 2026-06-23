#include "pca9535_button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pca9535_button {

static const char *const TAG = "pca9535_button";

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

  // Tijdelijk: log alle 8 bits van Port 1 individueel, zodat we zien
  // welk bit verandert wanneer de knop wordt ingedrukt.
  ESP_LOGD(TAG, "Port1 raw=0x%02X | bit0=%d bit1=%d bit2=%d bit3=%d bit4=%d bit5=%d bit6=%d bit7=%d",
           reg_val,
           (reg_val >> 0) & 1, (reg_val >> 1) & 1, (reg_val >> 2) & 1, (reg_val >> 3) & 1,
           (reg_val >> 4) & 1, (reg_val >> 5) & 1, (reg_val >> 6) & 1, (reg_val >> 7) & 1);

  bool pressed = !((reg_val >> BUTTON_BIT) & 0x01);

  if (this->binary_sensor_ != nullptr) {
    this->binary_sensor_->publish_state(pressed);
  }
}

}  // namespace pca9535_button
}  // namespace esphome
