# Quick Start

This document describes how to install and run the ROS Alate middleware.
The code was tested on Ubuntu 22.04 Raspberry Pi 4b, and Ubuntu 22.04 x86_64.
For other platforms, you may need to use docker. See the docker section for more details.

1. Copy archive to the target machine and extract it
    ```bash
    tar -xvf swarm_middleware.tar.gz -C . --strip-components=3
    ```
2. install dependencies. This step is required only once.
    ```bash
    cd swarm_middleware
    ./install_dependencies.sh
    ```

3. source the setup script. This step is required every time you open a new terminal.
Otherwise, you will get an error message saying that the package is not found.
    ```bash
    source /install_dir/setup.bash
    ```

It's recommended to add the above line to your .bashrc file so that you don't have to do it every time.
    ```bash
    echo "source /install_dir/setup.bash" >> ~/.bashrc
    ```

4. Run the demo for checking if everything is working fine.
    ```bash
    cd swarm_middleware_api/
    ./bin/demo ./examples/
    ```

## Using the API
1. Include the header file
    ```cpp
    #include "install_dir/swarm_middleware_api/swarm_middleware_api.hpp"
    ```

2. Compile
    ```bash
    g++ src/demo.cpp  -L./lib  -lswarm_middleware_api -o demo
    ```

## Docker

If you don't have docker installed, you can install it using the following command.
    ```bash
    ./docker/install_docker.sh
    ```

For Raspberry Pi,
    ```bash
    ./docker/install_docker_pi.sh
    ```

### Build the docker image

```bash
./docker/build_docker.sh <docker_dir_path>
```

for example,
```bash
./docker/build_docker.sh ~/dev/docker
```

### Run the docker image

```bash
./docker/run_docker.sh <swarm_middleware_api_dir_path>
```

for example,
```bash
./docker/run_docker.sh ~/dev/swarm_middleware_api
```

### Run the demo

```bash
./bin/demo ./examples/
```

## For ROS2 users

1. copy the ROS2 messages to your ROS2 workspace
    ```bash
    cp -r /messages_src/* ~/ros2_ws/
    ```

2. build the ROS2 messages
    ```bash
    cd ~/ros2_ws
    colcon build --symlink-install
    ```

3. source the ROS2 workspace
    ```bash
    source ~/dev/ros2_ws/install/setup.bash
    ```

## Running ROS Alate on ROS2 Humble Environment

```bash
cd ~/dev/PX4-Autopilot
make px4_sitl gazebo
```
    
```bash
ros2 run mission_control mission_control_state_machine
run high_level_control high_level_control_state_machine
ros2 run low_level_control_cpp low_level_control_node
```

takeoff
```bash
ros2 topic pub  /rc_state alate_interfaces/msg/RcEnum  "{value : 1}" --once
```

land
```bash
ros2 topic pub  /rc_state alate_interfaces/msg/RcEnum  "{value : 2}" --once
```

go home
```bash
ros2 topic pub  /rc_state alate_interfaces/msg/RcEnum  "{value : 3}" --once
```