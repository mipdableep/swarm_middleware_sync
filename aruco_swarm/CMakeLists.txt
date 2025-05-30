cmake_minimum_required(VERSION 3.1)
project(ArucoSwarm)

set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
# find_package(CTello REQUIRED)

include_directories(include)
include_directories(../include)
link_directories(../lib)

add_library(aruco src/aruco.cpp)
add_library(drone_client src/drone_client.cc)

target_link_libraries(
  aruco 
  -lpthread 
  ${OpenCV_LIBS} 
  ${Boost_LIBRARIES} 
  swarm_middleware_api)

add_executable(followAruco followAruco.cpp)

target_link_libraries(
  followAruco
  aruco
  ctello
  drone_client
  -lpthread
  nlohmann_json::nlohmann_json
  swarm_middleware_api)
