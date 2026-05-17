#include "lilygo_t5s3_pro.h"
#include "esphome/core/log.h"

namespace esphome {
namespace lilygo_t5s3_pro {

static const char *TAG="t5s3pro";

void LilygoT5S3Pro::setup() {

    ESP_LOGI(TAG,"setup");

    // HIER komt later:
    // epd_board_init()
    // tps65185_init()
    // pca9535_init()
}

void LilygoT5S3Pro::update() {

    ESP_LOGI(TAG,"render");

    this->do_update();

    // later:
    // epd_draw()
}

void LilygoT5S3Pro::dump_config() {

    ESP_LOGCONFIG(TAG,"LilyGO T5S3 Pro");
}

}
}
