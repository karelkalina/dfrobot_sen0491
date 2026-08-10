#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace dfrobot_sen0491 {

class DFRobotSEN0491Component : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_distance_sensor(sensor::Sensor *sensor) { distance_sensor_ = sensor; }

 protected:
  void process_frame_();

  sensor::Sensor *distance_sensor_{nullptr};

  static const uint8_t FRAME_SIZE = 34;
  uint8_t buffer_[FRAME_SIZE];
  uint8_t buffer_index_{0};
  bool header_found_{false};
};

}  // namespace dfrobot_sen0491
}  // namespace esphome
