cmake_minimum_required(VERSION 3.8)
project(swarm_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
# Enums
  "msg/MissionTypeEnum.msg"
  "msg/NavigationQualityEnum.msg"
  "msg/SensorPixelPitchEnum.msg"
  "msg/SensorResolutionEnum.msg"
  "msg/SensorUseEnum.msg"
  "msg/TargetTypeEnum.msg"
  "msg/ZoneShapeEnum.msg"
  "msg/SensorTypeEnum.msg"
  "msg/ZoneTypeEnum.msg"
# Primitives
  "msg/Zone.msg"
  "msg/TargetSize.msg"
  "msg/MissionTargetData.msg"
  "msg/RelativePoint.msg"
  "msg/PayloadCapability.msg"
  "msg/GeoPoint.msg"
  "msg/DroneSize.msg"
  "msg/Drone.msg"
  "msg/AtmosphericConditions.msg"
  "msg/Target.msg"
# Interfaces
  "msg/Mission.msg"
  "msg/PlannedMission.msg"
  "msg/InitNavigate.msg"
  "msg/Navigation.msg"  
  "msg/InitObstaclesAndDrones.msg"
  "msg/ObstaclesAndDrones.msg"
  "msg/DroneOrientationAndAttitude.msg"
  "msg/OperationalReport.msg"
# Services
  "srv/FlightControl.srv"
  "srv/MissionPlanning.srv"
  "srv/Navigation.srv"
  "srv/ObstacleAvoidance.srv"
)


ament_package()
