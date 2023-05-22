cmake_minimum_required(VERSION 3.1)

project(ArucoSwarm)

set(CMAKE_CXX_STANDARD 17)

include_directories(../../include)

# Specify the path to the directory containing the library
link_directories(../../lib)

# Create an executable target named "test" from the source file
add_executable(location location.cpp)
add_executable(test test.cpp)

find_package (Eigen3 3.4 REQUIRED NO_MODULE)
find_package (OpenCV REQUIRED)


# Link the "test" target against the "swarm_middleware_api" library
target_link_libraries(
    location
    -lpthread 
    swarm_middleware_api
    Eigen3::Eigen
    ${Boost_LIBRARIES}
)

target_link_libraries(
    test
    swarm_middleware_api
)
# g++ src/working_with_interfaces_demo.cpp -Iinclude  -Llib  -lswarm_middleware_api -o demo
# this line works for compiling the working_with_interfaces_demo.cpp file from the base dir.