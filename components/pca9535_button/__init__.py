import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c
from esphome.const import CONF_ID

CODEOWNERS = ["@you"]
DEPENDENCIES = ["i2c"]

pca9535_button_ns = cg.esphome_ns.namespace("pca9535_button")
PCA9535Button = pca9535_button_ns.class_(
    "PCA9535Button",
    cg.PollingComponent,
    i2c.I2CDevice,
    binary_sensor.BinarySensor,
)

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(PCA9535Button)
    .extend(cv.polling_component_schema("200ms"))
    .extend(i2c.i2c_device_schema(0x20))
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
