#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

// Legacy ADC driver (compatibel met epdiy, dat dezelfde driver gebruikt).
// We vermijden esp_adc_cal.h (verwijderd in nieuwere IDF) en doen een
// eenvoudige, ongecalibreerde conversie met een vaste Vref-aanname.
#include <driver/adc.h>
#include "epdiy.h"

namespace esphome {
namespace lilygo_t5_47_battery {

class Lilygot547Battery : public PollingComponent {
 public:
  sensor::Sensor *voltage{nullptr};

  void setup() override;
  void update() override;
  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage = voltage_sensor; }

 protected:
  void update_battery_voltage_();
};

}  // namespace lilygo_t5_47_battery
}  // namespace esphome
