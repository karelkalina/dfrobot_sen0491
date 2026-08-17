#include "dfrobot_sen0491.h"
#include "esphome/core/log.h"
#include <string.h>
#include <stdlib.h>

namespace esphome {
namespace dfrobot_sen0491 {

  static const char *const TAG = "dfrobot_sen0491";

  void DFRobotSEN0491Component::setup() {
    ESP_LOGCONFIG(TAG, "Setting up DFRobot SEN0491...");
  }

  void DFRobotSEN0491Component::dump_config() {
    ESP_LOGCONFIG(TAG, "DFRobot SEN0491:");
    this->check_uart_settings(115200);
  }

  void DFRobotSEN0491Component::loop() {
    while(this->available()){
      char ch;
      this->read_byte(reinterpret_cast<uint8_t*>(&ch));

      switch(this->parse_state_){
        case(0):
          if(ch == 'S'){
            this->buffer_[buffer_index_++] = ch;
            this->parse_state_ = 1;
          }
          break;
        case(1):
          if(ch == 't'){
            this->buffer_[buffer_index_++] = ch;
            this->parse_state_ = 2; 
          }else{
            this->parse_state_ = 0;
            this->buffer_index_ = 0;
            std::fill(this->buffer_, this->buffer_+this->FRAME_SIZE, ' ');
          }
          break;
        case(2):
          if(ch == 'a'){
            this->buffer_[buffer_index_++] = ch;
            this->parse_state_ = 3;
          }else{
            this->parse_state_ = 0;
            this->buffer_index_ = 0;
            std::fill(this->buffer_, this->buffer_+this->FRAME_SIZE, ' ');
          }
          break;
        case(3):
          if(this->buffer_index_ < this->FRAME_SIZE){
            this->buffer_[buffer_index_++] = ch;
          }if (this->buffer_index_ == this->FRAME_SIZE){
            std::string output = buff_to_string();     
            int dist = str_to_dist(output);
            this->distance_sensor_->publish_state(dist);
            ESP_LOGD(TAG, "%s", output.c_str());
            this->parse_state_ = 0;
            this->buffer_index_ = 0;
            std::fill(this->buffer_, this->buffer_+this->FRAME_SIZE, ' ');

          }
          break;
        default:
          this->parse_state_ = 0;
          this->buffer_index_ = 0;
          std::fill(this->buffer_, this->buffer_+this->FRAME_SIZE, '\0');
          break;
      }
    }
  }

  std::string DFRobotSEN0491Component::buff_to_string(){
    std::string out = "";
    for(int i = 0; i < this->FRAME_SIZE; i++){
      char c = this->buffer_[i];
      
      if(c>=32 && c <= 126){
        out += c;
      }else{
        out += ' ';
      }
    }
    return out;
  }
  int DFRobotSEN0491Component::str_to_dist(std::string str){
    int dist;
    int start_idx = 33;
    dist = atoi(str.substr(start_idx, 4).c_str());
    return dist;
  }
}  // namespace dfrobot_sen0491
}  // namespace esphome
