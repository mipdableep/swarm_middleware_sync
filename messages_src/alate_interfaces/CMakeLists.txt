cmake_minimum_required(VERSION 3.8)
project(alate_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(geometry_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Float.msg"
  "msg/String.msg"
  "msg/HlcStateEnum.msg"
  "msg/LlcStatus.msg"
  "msg/AgentStatus.msg"
  "msg/McEnum.msg"
  "msg/RcEnum.msg"
  "msg/PeerEnum.msg"
  "msg/Velocity.msg"
  "msg/AutopilotActionEnum.msg"
  "msg/AutopilotStateEnum.msg"
  "srv/Autopilot.srv"
  DEPENDENCIES geometry_msgs
)

  ament_package()
