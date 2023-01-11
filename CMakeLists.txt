cmake_minimum_required(VERSION 3.5)
project(ydlidar_test)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(ydlidar_sdk REQUIRED)

#Include directories
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/src  
  ${YDLIDAR_SDK_INCLUDE_DIRS})

#link library directories
link_directories(${YDLIDAR_SDK_LIBRARY_DIRS})

add_executable(ydlidar_driver src/ydlidar_ros_driver.cpp)
ament_target_dependencies(ydlidar_driver rclcpp std_msgs sensor_msgs std_srvs)
target_link_libraries(ydlidar_driver
  ${YDLIDAR_SDK_LIBRARIES}
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


set(msg_files
  "msg/LaserFan.msg"
)

rosidl_generate_interfaces(${PROJECT_NAME}
  ${msg_files}
  DEPENDENCIES builtin_interfaces
)

install(TARGETS
  ydlidar_driver
  DESTINATION lib/${PROJECT_NAME} ${YDLIDAR_SDK_LIBRARY_DIRS}
)

install(DIRECTORY launch param
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_dependencies(rosidl_default_runtime)
ament_package()
