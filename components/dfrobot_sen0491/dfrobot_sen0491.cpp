#include "dfrobot_sen0491.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dfrobot_sen0491 {

static const char *const TAG = "dfrobot_sen0491";
uint8_t Data[34] = {0};
static const uint8_t HEADER[] = {'S', 't', 
'a'};

void DFRobotSEN0491Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DFRobot SEN0491...");
}

void DFRobotSEN0491Component::dump_config() {
  ESP_LOGCONFIG(TAG, "DFRobot SEN0491:");
  LOG_SENSOR("  ", "Distance", this->distance_sensor_);
  this->check_uart_settings(115200);
}

void DFRobotSEN0491Component::loop() {
  while (this->available()) {
    uint8_t byte;
    this->read_byte(&byte);

    if (!this->header_found_) {
      // Shift bytes looking for header match
      if (this->buffer_index_ < 4) {
        this->buffer_[this->buffer_index_] = byte;
        if (this->buffer_[this->buffer_index_] == HEADER[this->buffer_index_]) {
          this->buffer_index_++;
          if (this->buffer_index_ == 4) {
            this->header_found_ = true;
          }
        } else {
          // Reset: check if this byte could be start of header
          this->buffer_index_ = (byte == HEADER[0]) ? 1 : 0;
          if (byte == HEADER[0]) {
            this->buffer_[0] = byte;
          }
        }
      }
    } else {
      this->buffer_[this->buffer_index_++] = byte;
      if (this->buffer_index_ == FRAME_SIZE) {
        this->process_frame_();
        this->buffer_index_ = 0;
        this->header_found_ = false;
      }
    }
  }
}

void DFRobotSEN0491Component::process_frame_() {

  uint8_t status = this->buffer_[11];
  uint16_t distance_mm = this->buffer_[8] | (this->buffer_[9] << 8);
  uint16_t signal_strength = this->buffer_[12] | (this->buffer_[13] << 8);

  if (status != 0) {
    ESP_LOGD(TAG, "Distance status not valid: %u", status);
    return;
  }

  if (this->distance_sensor_ != nullptr) {
    float distance_m = distance_mm / 1000.0f;
    this->distance_sensor_->publish_state(distance_m);
  }
}

}  // namespace dfrobot_sen0491
}  // namespace esphome
