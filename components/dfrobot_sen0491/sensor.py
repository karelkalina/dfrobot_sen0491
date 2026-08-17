import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_DISTANCE,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    UNIT_METER,
)
from . import dfrobot_sen0491_ns, DFRobotSEN0491Component

DEPENDENCIES = ["dfrobot_sen0491"]

# The internal ID used to link the sensor back to the hub
CONF_DFROBOT_SEN0491_ID = "dfrobot_sen0491_id"

# Schema definition for the sensor platform
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_DFROBOT_SEN0491_ID): cv.use_id(DFRobotSEN0491Component),
        cv.Optional(CONF_DISTANCE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_DISTANCE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_METER, 
        ),
    }
)

async def to_code(config):
    # Get the hub variable created in __init__.py
    hub = await cg.get_variable(config[CONF_DFROBOT_SEN0491_ID])
    
    # If the distance sensor is configured in YAML, generate the code for it
    if CONF_DISTANCE in config:
        sens = await sensor.new_sensor(config[CONF_DISTANCE])
        # This requires a 'set_distance_sensor' method in your C++ class
        cg.add(hub.set_distance_sensor(sens))