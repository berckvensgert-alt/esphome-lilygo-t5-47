#pragma once

#include "esphome/components/display/display_buffer.h"
#include "esphome/core/component.h"

namespace esphome {
namespace lilygo_t5s3_pro {

class LilygoT5S3Pro :
  public PollingComponent,
  public display::DisplayBuffer {

 public:

  void setup() override;
  void update() override;
  void dump_config() override;

  int get_width_internal() override { return 960; }
  int get_height_internal() override { return 540; }

};

}
}
