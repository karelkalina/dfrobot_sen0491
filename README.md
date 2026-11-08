# SEN0491 Distance Sensor

This repository contains the configuration and ROS 2 driver for the SEN0491 distance sensor.

## Directory Structure
- `firmware/`: Contains the ESPHome configuration (`atomlite-distance.yaml`) and related files for the [Atom Lite board](https://shop.m5stack.com/products/atom-lite-esp32-development-kit) with [Grove cable](https://docs.m5stack.com/en/accessory/cable/grove_cable) to the SEN0491 sensor.

- `ros_node/`: Contains the `sen0491_driver` ROS 2 package to interface with the sensor over HTTP.

## Sensor Wiring

| Color  | Label | Description                       |
| ------ | ----- | --------------------------------- |
| Red    | VCC   | power supply input positive pole  |
| Black  | GND   | power ground wire                 |
| Yellow | TX    | UART transmitting data line       |
| Green  | RX    | UART receiving data line          |
| White  | ALARM | alarm output line                 |

### Connection to M5 Atom Lite (Grove Port)

| Sensor Pin  | Grove Cable Color | Atom Lite Pin | Function |
| ----------- | ----------------- | ------------- | -------- |
| Red (VCC)   | Red               | 5V            | Power    |
| Black (GND) | Black             | GND           | Ground   |
| Yellow (TX) | Yellow            | GPIO26        | ESP32 RX |
| Green (RX)  | White             | GPIO32        | ESP32 TX |

## Build and Flash Instructions

```bash
pixi run compile
pixi run upload
```

## Network Configuration

The sensor is configured to use a static IP address (`192.168.105.67`) on a
private network, which credentials must be set in `firmware/secrets.yaml`.
To switch to a different network, update the `secrets.yaml` and provide Wi-Fi
credentials or use the ESPHome captive portal that will be available when the
device cannot connect to the configured network.

Format of `secrets.yaml`:
```yaml
wifi_ssid: "YourWiFiSSID"
wifi_password: "YourWiFiPassword"
```


## ROS 2 Node

The corresponding ROS 2 node for this sensor is `sen0491_node`. It polls the
ESPHome web server to retrieve distance measurements and publishes them as a
`sensor_msgs/Range` message.

- **Default IP:** `192.168.105.67`
- **ESPHome Endpoint:** `/sensor/distance`
- **Output Topic:** `/sen0491/distance` (Range in meters)
- **Status Topic:** `/sen0491/signal_status`
- **Frame ID:** `sen0491_link`

### Usage

Run the node using:
```bash
source <ROS Workspace>/install/setup.bash
ros2 run sen0491_driver sen0491_node --ros-args -p sensor_ip:=192.168.105.67
```

# Setup instruction
1. (Reccomended, not necesary) Connect the M5 Atom Lite to a pc and flash it with the factory firmware
2. Go to 'web.esphome.io', and connect to your desired wifi network. Check the device ip and modify 'firmware/atomlite-distance.yaml' and  'ros_node/sen0491_driver/sen0491_node.py' to reflect this
3. Compile and run the firmware using pixi 
'pixi run compile'
'pixi run upload --device {ip_address}'
4. Verify that the device runs properly, either by checking the web server, endpoint or ros node (if something isn't working, i reccommend debuging in this order)
