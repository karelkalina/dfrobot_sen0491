import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_DISTANCE,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    UNIT_MILLIMETER,
)
from . import dfrobot_sen0491_ns, DFRobotSEN0491Component

DEPENDENCIES = ["dfrobot_sen0491"]

# The internal ID used to link the sensor back to the hub
CONF_DFROBOT_SEN0491_ID = "dfrobot_sen0491_id"
CONF_SIGNAL_STATUS = "signal_status" # 1. Define the new YAML key

# Schema definition for the sensor platform
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_DFROBOT_SEN0491_ID): cv.use_id(DFRobotSEN0491Component),
        cv.Optional(CONF_DISTANCE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_DISTANCE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_MILLIMETER, 
        ),
        cv.Optional(CONF_SIGNAL_STATUS): sensor.sensor_schema(
            state_class=STATE_CLASS_MEASUREMENT,
            icon="mdi:signal", # Adds a nice default icon in Home Assistant
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

    if CONF_SIGNAL_STATUS in config:
        sens = await sensor.new_sensor(config[CONF_SIGNAL_STATUS])
        cg.add(hub.set_signal_status_sensor(sens))
        