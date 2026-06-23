import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_VOLTAGE,
    UNIT_VOLT,
    UNIT_PERCENT,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_BATTERY,
    STATE_CLASS_MEASUREMENT,
)
from . import BQ25896Battery, bq25896_ns

CONF_BQ25896_BATTERY_ID = "bq25896_battery_id"
CONF_PERCENTAGE = "percentage"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_BQ25896_BATTERY_ID): cv.use_id(BQ25896Battery),
        cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_PERCENTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_BATTERY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)


async def to_code(config):
    var = await cg.get_variable(config[CONF_BQ25896_BATTERY_ID])

    if CONF_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_VOLTAGE])
        cg.add(var.set_voltage_sensor(sens))

    if CONF_PERCENTAGE in config:
        sens = await sensor.new_sensor(config[CONF_PERCENTAGE])
        cg.add(var.set_percentage_sensor(sens))
