import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from . import BQ25896Battery

CONF_BQ25896_BATTERY_ID = "bq25896_battery_id"
CONF_STATUS = "status"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_BQ25896_BATTERY_ID): cv.use_id(BQ25896Battery),
        cv.Optional(CONF_STATUS): text_sensor.text_sensor_schema(),
    }
)


async def to_code(config):
    var = await cg.get_variable(config[CONF_BQ25896_BATTERY_ID])

    if CONF_STATUS in config:
        sens = await text_sensor.new_text_sensor(config[CONF_STATUS])
        cg.add(var.set_status_sensor(sens))
