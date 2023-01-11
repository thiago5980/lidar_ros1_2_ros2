import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
  param_dir = LaunchConfiguration(
    'param_dir',
    default=os.path.join(
      get_package_share_directory('ydlidar_test'),
      'param',
      'lidar.yaml'))
  return LaunchDescription([
    DeclareLaunchArgument(
      'param_dir',
      default_value=param_dir,
      description='Full path of parameter file'
    ),
    Node(
      package='ydlidar_test',
      executable='ydlidar_driver',
      name='ydlidar_driver',
      parameters=[param_dir],
      output='screen'),
    Node(
      package='tf2_ros',
      node_executable='static_transform_publisher',
      node_name='static_tf_pub_laser',
      arguments=['0','0','0.02','0','0','0','1','base_link','laser_frame'],
      )
])