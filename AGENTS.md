# AGENTS.md - DFRobot SEN0491 ESPHome Project

## Project Overview
This repository contains an ESPHome firmware project running on an **M5Stack Atom Lite (ESP32)** configured to read distance and signal metrics from a **DFRobot SEN0491** distance sensor over UART (GPIO32 TX, GPIO26 RX).

The project is packaged and managed using **Pixi** for reproducible environment management and task execution.

---

## Directory Structure

```
/mnt/c/KM/sen0491/
├── pixi.toml                    # Environment specifications, dependencies, and task runners
├── atomlite-distance.yaml       # Main ESPHome device configuration file
├── components/
│   └── dfrobot_sen0491/         # Custom external ESPHome component
│       ├── __init__.py          # Hub component schema and UART device registration
│       ├── sensor.py            # Sensor platform schema for distance & signal status
│       ├── dfrobot_sen0491.h    # C++ Component header & parser state definitions
│       └── dfrobot_sen0491.cpp  # C++ UART stream byte-by-byte parser and state publisher
└── AGENTS.md                    # Agent guidelines and architectural documentation
```

---

## Development Environment & Tasks

All commands should be executed via `pixi`:

| Task | Command | Description |
| :--- | :--- | :--- |
| **Compile** | `pixi run compile` | Validates YAML and compiles the ESP32 firmware |
| **Upload** | `pixi run upload` | Flashes the firmware to the connected device |
| **Run** | `pixi run run` | Compiles, uploads, and starts serial/network logs |
| **Logs** | `pixi run logs` | Streams logs from the device |
| **Clean** | `pixi run clean` | Cleans ESPHome build artifacts |

---

## Component Architecture & Conventions

### 1. Python Validation & Codegen (`__init__.py`, `sensor.py`)
- Define schemas using `esphome.config_validation` (`cv`).
- Register components with `esphome.codegen` (`cg`).
- Always support optional sensor entities gracefully using `cv.Optional`.

### 2. C++ Component Lifecycle (`dfrobot_sen0491.h`, `.cpp`)
- Inherit from `Component` and `uart::UARTDevice`.
- Use non-blocking byte-by-byte stream parsing in `loop()`.
- **Null Safety**: Always check if pointers (e.g., `distance_sensor_ != nullptr`, `signal_status_sensor_ != nullptr`) are valid before calling `publish_state()`.
- **Logging**: Use ESPHome standard logging macros (`ESP_LOGD`, `ESP_LOGW`, `ESP_LOGE`, `ESP_LOGCONFIG`) with `TAG = "dfrobot_sen0491"`.

### 3. YAML Configuration (`atomlite-distance.yaml`)
- Pin assignments: UART TX `GPIO32`, RX `GPIO26`, Baud `115200`.
- Sensor entities can be exposed or marked `internal: true` depending on upstream requirements (e.g., Home Assistant, REST API, or custom JSON template sensors).

---

## Guidelines for Agents Modifying this Codebase
1. Run `pixi run compile` after any C++, Python, or YAML modifications to verify build integrity.
2. Ensure memory safety and avoid dynamic buffer overruns in the UART parsing loop.
3. Preserve reproducible dependencies in `pixi.toml`.
