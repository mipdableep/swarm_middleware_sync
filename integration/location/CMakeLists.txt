cmake_minimum_required(VERSION 3.1)

project(ArucoSwarm)

set(CMAKE_CXX_STANDARD 17)

include_directories(../../include)

link_directories(../../lib)

# Create an executable target named "vel_sender" from the source file

find_package (Eigen3 3.4 REQUIRED NO_MODULE)
find_package (OpenCV REQUIRED)


add_executable(location location.cpp)
target_link_libraries(
    location
    -lpthread 
    swarm_middleware_api
    Eigen3::Eigen
    ${Boost_LIBRARIES}
    )

add_executable(vel_sender vel_sender.cpp)
target_link_libraries(
    vel_sender
    swarm_middleware_api
)
# g++ src/working_with_interfaces_demo.cpp -Iinclude  -Llib  -lswarm_middleware_api -o demo
# this line works for compiling the working_with_interfaces_demo.cpp file from the base dir.