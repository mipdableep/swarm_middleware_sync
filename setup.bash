# Add the middleware environment variables to current shell
export ROS_VERSION=2
export ROS_PYTHON_VERSION=3
export ROS_DISTRO=humble
export RMW_IMPLEMENTATION=rmw_fastrtps_cpp

# Set the domain ID (default is 0)
export ROS_DOMAIN_ID=0

# Limit communication to localhost only (default is 0) 
# 1: localhost only, 0: all interfaces
export ROS_LOCALHOST_ONLY=0

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
# compilation and runtime
export CPLUS_INCLUDE_PATH=$SCRIPT_PATH/include:$CPLUS_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$SCRIPT_PATH/packages/swarm_interfaces/include:$CPLUS_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$SCRIPT_PATH/packages/alate_interfaces/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$SCRIPT_PATH/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$SCRIPT_PATH/runtime_lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$SCRIPT_PATH/packages/swarm_interfaces/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$SCRIPT_PATH/packages/alate_interfaces/lib:$LD_LIBRARY_PATH

# interface packages
export AMENT_PREFIX_PATH=$SCRIPT_PATH/packages/swarm_interfaces:$AMENT_PREFIX_PATH
export AMENT_PREFIX_PATH=$SCRIPT_PATH/packages/alate_interfaces:$AMENT_PREFIX_PATH
export AMENT_CURRENT_PREFIX=$SCRIPT_PATH
