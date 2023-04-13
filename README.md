# ROS-alate sync - aruco swarm
this repo contains precompiled files for aarch64v8 linux docker container, and lets the user
make use of the ros-alate system on a raspberry pi
## docker
### general
get the ROS-alate_v1_3.tar file. this file contains a docker container environment that has all of the prerequisites for this project installed (some through linux mirrors, and some compiled by hand). 

+ **this environment is intended for use _only on aarch64_ systems such as pi0w2 and pi4b, and will not work on other architectures**

### install environment
given the ROS-alate_v1_3.tar file, you can add it to the raspberry pi's docker images with 
```sh
sudo docker load ./ROS-alate_v1_3.tar
```
and tag it as ros-alate v1.3 with:
```sh
sudo docker images
# find the image without a name, and copy its image ID, then:
sudo docker tag <imageID> ros-alate:1.3
```
after which you should see it in your docker images.  
### enter image
to enter the image, cd to the main folder and run
```sh
sudo ./docker/run_docker ./
```
this will mount the current directory onto `/swarm_middleware` inside of the docker container

### test precompiled examples
you always need to execute `source ./setup.sh` before running.  
this line should be in the .bashrc file and so should be executed automatically, but check it if you encounter problems.  
  
to test if the system is working correctly, you can try to run the `bin/working_with_interfaces_demo` file with
```sh
./bin/working_with_interfaces_demo ./examples
```

## ros-alate system
### include and compile
you can use the ros-alate interface by adding this line to a c++ file:
```cpp
#include "include/swarm_middleware_api/node.hpp"
```

and to compile use 
```sh
g++ file.cpp -Include -Llib  -lswarm_middleware_api -o output
```

for cmake example see this [cmake lists file](./src/CMakeLists.txt).

### usage
[working_with_interfaces_demo.cpp](./src/working_with_interfaces_demo.cpp) shows the main gist of how to work with the Node class, 
and the .yaml files at [examples](./examples) show how to format the messages correctly.

## aruco swarm on system
to run the project, cd to base dir and `./runPipe.sh &`.
enter docker,cd to aruco_swarm/build/, and execute the [followAruco](aruco_swarm/build/followAruco) file

+ **will only work as intended on rpiLite 64 os**

you can change the parameters of the run in the [aruco_swarm/config](aruco_swarm/config) file, to help debugging and changing running conditions.