cmake_minimum_required(VERSION 3.1)

project(ArucoSwarm)

set(CMAKE_CXX_STANDARD 17)

include_directories(../../include)
link_directories(../../lib)

find_package (OpenCV REQUIRED)
find_package (Boost REQUIRED)

add_executable(peer_detector peer_detection.cpp)

target_link_libraries(
    peer_detector
    swarm_middleware_api
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    )

# g++ src/working_with_interfaces_demo.cpp -Iinclude  -Llib  -lswarm_middleware_api -o demo
# this line works for compiling the working_with_interfaces_demo.cpp file from the base dir.