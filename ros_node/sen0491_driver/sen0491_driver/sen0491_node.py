#!/usr/bin/env python3
import json
import requests
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Range
from std_msgs.msg import String

class Sen0491Node(Node):
    def __init__(self):
        super().__init__('sen0491_node')
        self.declare_parameter('sensor_ip', '192.168.105.67')
        self.declare_parameter('frame_id', 'sen0491_link')
        self.declare_parameter('poll_period', 0.1)

        self.sensor_ip = self.get_parameter('sensor_ip').get_parameter_value().string_value
        self.frame_id = self.get_parameter('frame_id').get_parameter_value().string_value
        poll_period = self.get_parameter('poll_period').get_parameter_value().double_value

        self.url = f'http://{self.sensor_ip}/text_sensor/paired_sensor_data'
        self.range_pub = self.create_publisher(Range, '/sen0491/distance', 10)
        self.status_pub = self.create_publisher(String, '/sen0491/signal_status', 10)
        self.timer = self.create_timer(poll_period, self.timer_callback)

        self.get_logger().info(f'SEN0491 node started: {self.url}')

    def timer_callback(self):
        try:
            response = requests.get(self.url, timeout=(1.0, 2.0))
            response.raise_for_status()
            data = response.json()
            state_str = data.get('value', data.get('state', ''))
            
            if not state_str: return
            sensor_data = json.loads(state_str)
            if sensor_data.get('status') == 'booting': return

            distance_mm = float(sensor_data['distance'])
            distance_m = distance_mm / 1000.0

            msg = Range()
            msg.header.stamp = self.get_clock().now().to_msg()
            msg.header.frame_id = self.frame_id
            msg.radiation_type = Range.INFRARED
            msg.field_of_view = 0.05
            msg.min_range = 0.0
            msg.max_range = 50.0
            msg.range = distance_m

            self.range_pub.publish(msg)

            status_msg = String()
            status_msg.data = str(sensor_data.get('signal_status', ''))
            self.status_pub.publish(status_msg)
        except Exception as e:
            self.get_logger().warn(f'Error: {e}', throttle_duration_sec=2.0)

def main(args=None):
    rclpy.init(args=args)
    node = Sen0491Node()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
