#include "../../include/swarm_middleware_api/node.hpp"
#include <iostream>
#include <unistd.h>

void print_msg_callback(const std::string &msg)
{
    std::cout << "Received message:\n"
              << msg << std::endl;
}

std::string make_msg(int x, int y, int z, int Rz)
{
    return ("linear_x: " + std::to_string(x) + 
          "\nlinear_y: " + std::to_string(y) +
          "\nlinear_z: " + std::to_string(z) +
          "\npitch: 0.0\nroll: 0.0\nyaw: " + std::to_string(Rz));
}

int main(int argc, char **argv)
{
    using ros_alate::InterfaceType;
    using ros_alate::Node;
    using ros_alate::QosSettings;
    using ros_alate::ReliabilityQosEnum;
 
    auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};

    auto const NODE_NAME = std::string("ros_alate_middeware_demo");
    auto const INTERFACES = std::vector<std::string>{"swarm_interfaces", "alate_interfaces"};
    auto user_api = Node(argc, argv, NODE_NAME, INTERFACES);

    auto interface_type_vel = InterfaceType("alate_interfaces", "Velocity");
    auto interface_type_drone_alt = InterfaceType("swarm_interfaces", "DroneOrientationAndAttitude");

    user_api.listen("velocity_t", interface_type_vel, qos, print_msg_callback);
    user_api.listen("location", interface_type_drone_alt, qos, print_msg_callback);
    user_api.set_advertiser("velocity_t", interface_type_vel, qos);
    user_api.advertise("velocity_t", make_msg(0,0,0,0));
    user_api.spinOnce();
    usleep(20000);
    user_api.advertise("velocity_t", make_msg(0,0,0,0));
    user_api.spinOnce();
    usleep(20000);
    user_api.advertise("velocity_t", make_msg(0,0,0,0));
    user_api.spinOnce();
    usleep(20000);
    user_api.advertise("velocity_t", make_msg(0,0,0,0));
    user_api.spinOnce();
    usleep(20000);
    user_api.advertise("velocity_t", make_msg(0,0,0,0));
    user_api.spinOnce();
    usleep(20000);
    user_api.advertise("velocity_t", make_msg(0,0,0,0));
    user_api.spinOnce();
    usleep(20000);

}