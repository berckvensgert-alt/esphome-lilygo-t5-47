import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, text_sensor, i2c
from esphome.const import (
    CONF_ID,
    CONF_VOLTAGE,
    UNIT_VOLT,
    UNIT_PERCENT,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_BATTERY,
    STATE_CLASS_MEASUREMENT,
    ICON_BATTERY,
)

CONF_PERCENTAGE = "percentage"
CONF_STATUS = "status"

bq25896_ns = cg.esphome_ns.namespace("bq25896_battery")
BQ25896Battery = bq25896_ns.class_(
    "BQ25896Battery", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BQ25896Battery),
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
        cv.Optional(CONF_STATUS): text_sensor.text_sensor_schema(
            icon=ICON_BATTERY,
        ),
    }
).extend(cv.polling_component_schema("5s")).extend(i2c.i2c_device_schema(0x6B))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_VOLTAGE])
        cg.add(var.set_voltage_sensor(sens))

    if CONF_PERCENTAGE in config:
        sens = await sensor.new_sensor(config[CONF_PERCENTAGE])
        cg.add(var.set_percentage_sensor(sens))

    if CONF_STATUS in config:
        sens = await text_sensor.new_text_sensor(config[CONF_STATUS])
        cg.add(var.set_status_sensor(sens))
