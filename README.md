# ROS-alate sync - aruco swarm
this repo contains precompiled files for aarch64v8 linux docker container, and lets the user
make use of the ros-alate system on a raspberry pi
## system summery
this repo contains the ROS_ALATE system produced by Matrix-defense, and the aruco-swarm project, with modifications to allow
it to run while sending and receiving messages from the ros-alate system using the provided docker container.
 
the aruco swarm project runs as the "brain" of a tello drone, and guides it to keep a formation with other drones (each running the program separately), while receiving input only from the onboard camera of the tello.
## docker
### general
get the ROS-alate_v1_3.tar file. this file contains a docker container environment that has all of the prerequisites for this project installed (some through linux mirrors, and some compiled by hand). 

+ **this environment is intended for use _only on aarch64_ systems such as pi0w2 and pi4b, and will not work on other architectures**

### install environment
you can check if the docker image is installed with `sudo docker images`  
  

if the image is not installed given the ROS-alate_v1_3.tar file, you can add it to the raspberry pi's docker images with 
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
the version of the aruco-swarm project provided in this repo is one extensively modified to work with the Ros-alate docker.  

to run the project, cd to base dir and `./runPipe.sh &`.
enter docker,cd to aruco_swarm/build/, and execute the [followAruco](aruco_swarm/build/followAruco) file.  

if the executable does not exist, cd to aruco_swarm/build/, and run `cmake .. && make`  

+ **will only work as intended on rpiLite 64 os**  

you can change the parameters of the run in the [aruco_swarm/config](aruco_swarm/config) file, to help debugging and changing running conditions.

to run multiple drones at once in sync, change the config.json to true for the `run_server` option.  
that will make it so the drone will wait for a takeoff command from a server before taking off and starting.
do it for each drone and then from the device configured as the "server" in the configure file run:
```sh
python3 base/aruco_swarm/server.py 0.0.0.0 9090 <number of drones>
```