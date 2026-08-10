import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, CONF_UART_ID

CODEOWNERS = ["@vinsce"]
DEPENDENCIES = [ ]
AUTO_LOAD = [ ]
MULTI_CONF = False

# C++ namespace
dfrobot_sen0491_ns = cg.esphome_ns.namespace("dfrobot_sen0491")
DFRobotSEN0491Component = dfrobot_sen0491_ns.class_("DFRobotSEN0491Component", cg.Component)
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotSEN0491Component),
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    # Schema definition, containing the options available for the component
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)    
    pass