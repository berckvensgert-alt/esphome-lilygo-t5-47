#pragma once
#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pca9535_button {

class PCA9535Button : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;

  void set_binary_sensor(binary_sensor::BinarySensor *s) { binary_sensor_ = s; }

 protected:
  binary_sensor::BinarySensor *binary_sensor_{nullptr};
};

}  // namespace pca9535_button
}  // namespace esphome
