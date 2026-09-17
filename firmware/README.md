# SEN0491 (Laser) Sensor Node

This directory contains the ESPHome configuration for the DFRobot SEN0491 Laser distance sensor.

## ROS 2 Node
The corresponding ROS 2 node for this sensor is `sen0491_node`. It polls a custom `text_sensor` endpoint on the ESPHome web server and parses JSON to publish distance and signal status.

- **Default IP:** `192.168.105.67`
- **ESPHome Endpoint:** `/text_sensor/paired_sensor_data`
- **Output Topics:** 
  - `/sen0491/distance` (`sensor_msgs/Range` in meters)
  - `/sen0491/signal_status` (`std_msgs/String`)
- **Frame ID:** `sen0491_link`

### Usage
Run the node using:
```bash
source /mnt/c/KM/ROS2-Sensor-Nodes-RVZ/install/setup.bash
ros2 run rovozci_glsensor sen0491_node --ros-args -p sensor_ip:="192.168.105.67"
```
