import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from . import PCA9535Button

CONF_PCA9535_BUTTON_ID = "pca9535_button_id"

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema().extend(
    {
        cv.GenerateID(CONF_PCA9535_BUTTON_ID): cv.use_id(PCA9535Button),
    }
)


async def to_code(config):
    var = await cg.get_variable(config[CONF_PCA9535_BUTTON_ID])
    sens = await binary_sensor.new_binary_sensor(config)
    cg.add(var.set_binary_sensor(sens))
