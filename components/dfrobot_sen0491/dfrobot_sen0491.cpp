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
            this->reset_buffer();
          }
          break;
        case(2):
          if(ch == 'a'){
            this->buffer_[buffer_index_++] = ch;
            this->parse_state_ = 3;
          }else{
            this->reset_buffer();
          }
          break;
        case(3):
          if(this->buffer_index_ < this->FRAME_SIZE){
            this->buffer_[buffer_index_++] = ch;
          }
          if (this->buffer_index_ == this->FRAME_SIZE){
            std::string output = buff_to_string();     
            int dist = str_to_dist(output);
            int signal_status = str_to_sigs(output);
            if (this->distance_sensor_ != nullptr) {
              this->distance_sensor_->publish_state(dist);
            }
            if (this->signal_status_sensor_ != nullptr) {
              this->signal_status_sensor_->publish_state(signal_status);
            }
            
            /*
            debug logging */
            ESP_LOGD(TAG, "%s", output.c_str());
           
            this->reset_buffer();

          }
          break;
        default:
          this->reset_buffer();
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
    int dist = 0;
    int start_idx = 33;
    if (str.length() >= start_idx + 4) {
      dist = atoi(str.substr(start_idx, 4).c_str());
    }
    return dist;
  }
  int DFRobotSEN0491Component::str_to_sigs(std::string str){
    int sig_stat = 1;
    std::string substr = "Range Valid";
    if (str.find(substr) != std::string::npos) {
      sig_stat = 0;
    }
    return sig_stat;
  }
  void DFRobotSEN0491Component::reset_buffer(){
    this->parse_state_ = 0;
    this->buffer_index_ = 0;
    std::fill(this->buffer_, this->buffer_+this->FRAME_SIZE, '\0');
  }

}  // namespace dfrobot_sen0491
}  // namespace esphome
