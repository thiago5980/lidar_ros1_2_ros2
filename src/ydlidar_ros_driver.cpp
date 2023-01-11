#include "rclcpp/rclcpp.hpp"

#include "ydlidar_config.h"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/point_cloud.hpp"
#include "std_srvs/srv/empty.hpp"
#include "src/CYdLidar.h"

#include <memory>
#include <limits>
#include <iostream>
#include <vector>

#define SDKROSVerision "2.0.1"
using namespace std;

int main(int argc, char * argv[])
{
  CYdLidar laser;
  RCLCPP_INFO(rclcpp::get_logger("info"),"YDLIDAR ROS Driver Version: %s", SDKROSVerision);
  
  rclcpp::init(argc, argv);
  auto _node = rclcpp::Node::make_shared("ydlidar_test_node");
  auto scan_pub = _node->create_publisher<sensor_msgs::msg::LaserScan>("scan",1);
  auto pc_pub = _node->create_publisher<sensor_msgs::msg::PointCloud>("point_cloud", 1);

  _node->declare_parameter("port", "/dev/ttyUSB0");
  string port = _node->get_parameter("port").get_value<string>();
  laser.setlidaropt(LidarPropSerialPort, port.c_str(), port.size());
  
  _node->declare_parameter("ignore_array", "");
  string ignore_array = _node->get_parameter("ignore_array").get_value<string>();
  laser.setlidaropt(LidarPropIgnoreArray, ignore_array.c_str(), ignore_array.size());

  _node->declare_parameter("frame_id", "laser_frame");
  string frame_id = _node->get_parameter("frame_id").get_value<string>();

  _node->declare_parameter("baudrate", 230400);
  int baudrate = _node->get_parameter("baudrate").get_value<int>();
  laser.setlidaropt(LidarPropSerialBaudrate, &baudrate, sizeof(int));

  _node->declare_parameter("lidar_type", 1);
  int lidar_type = _node->get_parameter("lidar_type").get_value<int>();
  laser.setlidaropt(LidarPropLidarType, &lidar_type, sizeof(int));

  _node->declare_parameter("device_type", 0);
  int device_type = _node->get_parameter("device_type").get_value<int>();
  laser.setlidaropt(LidarPropDeviceType, &device_type, sizeof(int));

  _node->declare_parameter("sample_rate", 9);
  int sample_rate = _node->get_parameter("sample_rate").get_value<int>();
  laser.setlidaropt(LidarPropSampleRate, &sample_rate, sizeof(int));

  _node->declare_parameter("abnormal_check_count", 4);
  int abnormal_check_count = _node->get_parameter("abnormal_check_count").get_value<int>();
  laser.setlidaropt(LidarPropAbnormalCheckCount, &abnormal_check_count, sizeof(int));

  _node->declare_parameter("resolution_fixed", true);
  int resolution_fixed = _node->get_parameter("resolution_fixed").get_value<bool>();
  laser.setlidaropt(LidarPropFixedResolution, &resolution_fixed, sizeof(bool));

  _node->declare_parameter("auto_reconnect", true);
  int auto_reconnect = _node->get_parameter("auto_reconnect").get_value<bool>();
  laser.setlidaropt(LidarPropAutoReconnect, &auto_reconnect, sizeof(bool));

  _node->declare_parameter("reversion", true);
  int reversion = _node->get_parameter("reversion").get_value<bool>();
  laser.setlidaropt(LidarPropReversion, &reversion, sizeof(bool));

  _node->declare_parameter("inverted", true);
  int inverted = _node->get_parameter("inverted").get_value<bool>();
  laser.setlidaropt(LidarPropInverted, &inverted, sizeof(bool));

  _node->declare_parameter("isSingleChannel", false);
  int isSingleChannel = _node->get_parameter("isSingleChannel").get_value<bool>();
  laser.setlidaropt(LidarPropSingleChannel, &isSingleChannel, sizeof(bool));

  _node->declare_parameter("intensity", true);
  int intensity = _node->get_parameter("intensity").get_value<bool>();
  laser.setlidaropt(LidarPropIntenstiy, &intensity, sizeof(bool));

  _node->declare_parameter("support_motor_dtr", false);
  int support_motor_dtr = _node->get_parameter("support_motor_dtr").get_value<bool>();
  laser.setlidaropt(LidarPropSupportMotorDtrCtrl, &support_motor_dtr, sizeof(bool));

  _node->declare_parameter("angle_min", -180.f);
  float angle_min = _node->get_parameter("angle_min").get_value<float>();
  laser.setlidaropt(LidarPropMinAngle, &angle_min, sizeof(float));

  _node->declare_parameter("angle_max", 180.f);
  float angle_max = _node->get_parameter("angle_max").get_value<float>();
  laser.setlidaropt(LidarPropMinAngle, &angle_max, sizeof(float));

  _node->declare_parameter("range_min", 0.1f);
  float range_min = _node->get_parameter("range_min").get_value<float>();
  laser.setlidaropt(LidarPropMinRange, &range_min, sizeof(float));

  _node->declare_parameter("range_max", 16.f);
  float range_max = _node->get_parameter("range_max").get_value<float>();
  laser.setlidaropt(LidarPropMaxAngle, &range_max, sizeof(float));

  _node->declare_parameter("frequency", 10.f);
  float frequency = _node->get_parameter("frequency").get_value<float>();
  laser.setlidaropt(LidarPropScanFrequency, &frequency, sizeof(float));

  _node->declare_parameter("invalid_range_is_inf", false);
  bool invalid_range_is_inf = _node->get_parameter("invalid_range_is_inf").get_value<bool>();  

  _node->declare_parameter("point_cloud_preservative", false);
  bool point_cloud_preservative = _node->get_parameter("point_cloud_preservative").get_value<bool>();  

  auto stop_scan = 
    [&laser](const std::shared_ptr<std_srvs::srv::Empty::Request> req,
               std::shared_ptr<std_srvs::srv::Empty::Response> res)->bool{
      RCLCPP_DEBUG(rclcpp::get_logger("rclcpp"), "Stop scan");
      return laser.turnOff();
  };

  auto start_scan = 
    [&laser](const shared_ptr<std_srvs::srv::Empty::Request> req,
                shared_ptr<std_srvs::srv::Empty::Response> res)->bool{
      RCLCPP_DEBUG(rclcpp::get_logger("rclcpp"), "Start scan");
      return laser.turnOn();    
  };

  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr stop_scan_service = 
    _node->create_service<std_srvs::srv::Empty>("stop_scan", stop_scan);

  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr start_scan_service = 
    _node->create_service<std_srvs::srv::Empty>("start_scan", start_scan);

  bool ret = laser.initialize();

  if(ret){
    ret = laser.turnOn();
  } else{
    RCLCPP_ERROR(_node->get_logger(), "%s\n", laser.DescribeError());
  }
  
  rclcpp::Rate loop_rate(30);

  while(ret && rclcpp::ok()){
    LaserScan scan;

    if(laser.doProcessSimple(scan)){
      sensor_msgs::msg::LaserScan scan_msg;
      sensor_msgs::msg::PointCloud pc_msg;

      rclcpp::Time start_scan_time;
      scan_msg.header.stamp.sec = scan.stamp / 1000000000ul;
      scan_msg.header.stamp.nanosec = scan.stamp % 1000000000ul;
      scan_msg.header.frame_id = frame_id;
      pc_msg.header = scan_msg.header;
      scan_msg.angle_min = (scan.config.min_range);
      scan_msg.angle_max = (scan.config.max_angle);
      scan_msg.angle_increment = (scan.config.angle_increment);
      scan_msg.scan_time = scan.config.scan_time;
      scan_msg.time_increment = scan.config.time_increment;
      scan_msg.range_min = (scan.config.min_range);
      scan_msg.range_max = (scan.config.max_range);
      
      int size = (scan.config.max_angle - scan.config.min_angle) / 
                  scan.config.angle_increment + 1;
      scan_msg.ranges.resize(size,
                             invalid_range_is_inf ? std::numeric_limits<float>::infinity() : 0.0);
      scan_msg.intensities.resize(size);
      pc_msg.channels.resize(2);
      int idx_intensity = 0;
      pc_msg.channels[idx_intensity].name = "intensities";
      int idx_timestamp = 1;
      pc_msg.channels[idx_timestamp].name = "stamps";

      for(size_t i = 0; i < scan.points.size(); i++){
        int index = std::ceil((scan.points[i].angle - scan.config.min_angle) / 
                              scan.config.angle_increment);
        if(index>0 && index < size){
          if(scan.points[i].range >= scan.config.min_angle){
            scan_msg.ranges[index] = scan.points[i].range;
            scan_msg.intensities[index] = scan.points[i].intensity;
          }
        }
        if(point_cloud_preservative ||
          (scan.points[i].range >= scan.config.min_angle && 
           scan.points[i].range <= scan.config.max_angle)){
          geometry_msgs::msg::Point32 point;
          point.x = scan.points[i].range * cos(scan.points[i].angle);
          point.y = scan.points[i].range * sin(scan.points[i].angle);
          point.z = 0.0;
          pc_msg.points.push_back(point);
          pc_msg.channels[idx_intensity].values.push_back(scan.points[i].intensity);
          pc_msg.channels[idx_timestamp].values.push_back(i * scan.config.time_increment);                    
        }
      }

      scan_pub->publish(scan_msg);
      pc_pub->publish(pc_msg);
      } else{
        RCLCPP_ERROR(_node->get_logger(),"Failed to get Lidar Data");
      }

    rclcpp::spin_some(_node);
    loop_rate.sleep();
  }
  
  laser.turnOff();
  RCLCPP_INFO(rclcpp::get_logger("shutdown"), "[YDLIDAR INFO] Now YDLIDAR is stopping .....");
  laser.disconnecting();
  rclcpp::shutdown();
  return 0;
}