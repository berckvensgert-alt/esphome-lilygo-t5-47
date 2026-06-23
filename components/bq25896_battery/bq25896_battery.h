#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace bq25896_battery {

class BQ25896Battery : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;

  void set_voltage_sensor(sensor::Sensor *s) { voltage_sensor_ = s; }
  void set_percentage_sensor(sensor::Sensor *s) { percentage_sensor_ = s; }
  void set_status_sensor(text_sensor::TextSensor *s) { status_sensor_ = s; }

 protected:
  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *percentage_sensor_{nullptr};
  text_sensor::TextSensor *status_sensor_{nullptr};

  float read_battery_voltage_();
  float voltage_to_percentage_(float voltage);
  std::string read_charge_status_();
};

}  // namespace bq25896_battery
}  // namespace esphome
