import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display

DEPENDENCIES = ["spi"]

lilygo_ns = cg.esphome_ns.namespace("lilygo_t5s3_pro")

LilygoT5S3Pro = lilygo_ns.class_(
    "LilygoT5S3Pro",
    cg.PollingComponent,
    display.DisplayBuffer
)

CONFIG_SCHEMA = display.FULL_DISPLAY_SCHEMA.extend(
{
    cv.GenerateID(): cv.declare_id(LilygoT5S3Pro),
})

async def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])

    await cg.register_component(var, config)
    await display.register_display(var, config)
