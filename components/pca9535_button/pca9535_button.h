#pragma once
#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace pca9535_button {

class PCA9535Button : public PollingComponent, public i2c::I2CDevice, public binary_sensor::BinarySensor {
 public:
  void setup() override;
  void update() override;
};

}  // namespace pca9535_button
}  // namespace esphome
