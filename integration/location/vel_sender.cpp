#include "../../include/swarm_middleware_api/node.hpp"
#include <iostream>
#include <unistd.h>

auto const NODE_NAME = std::string("ros_test");
auto const INTERFACES = std::vector<std::string>{"swarm_interfaces", "alate_interfaces"};

ros_alate::QosSettings qos = ros_alate::QosSettings{ros_alate::ReliabilityQosEnum::BEST_EFFORT, 10};
ros_alate::InterfaceType interface_type_drone_alt = ros_alate::InterfaceType("swarm_interfaces", "DroneOrientationAndAttitude");
ros_alate::InterfaceType interface_type_vel = ros_alate::InterfaceType("alate_interfaces", "Velocity");

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
    if (argc < 5) {
        std::cout << "Please provide four integer arguments." << std::endl;
        return 1;
    }

    int v1 = std::stoi(argv[1]);
    int v2 = std::stoi(argv[2]);
    int v3 = std::stoi(argv[3]);
    int v4 = std::stoi(argv[4]);

    using ros_alate::InterfaceType;
    using ros_alate::Node;
    using ros_alate::QosSettings;
    using ros_alate::ReliabilityQosEnum;
 
    auto user_api = Node(argc, argv, NODE_NAME, INTERFACES);
    user_api.set_advertiser("velocity_t", interface_type_vel, qos);
    sleep (5);

    user_api.listen("velocity_t", interface_type_vel, qos, print_msg_callback);
    user_api.listen("location", interface_type_drone_alt, qos, print_msg_callback);
    user_api.advertise("velocity_t", make_msg(v1, v2, v3, v4));
    user_api.spinOnce();
    usleep(20000);
    user_api.advertise("velocity_t", make_msg(v1, v2, v3, v4));
    user_api.spinOnce();
    usleep(20000);

}