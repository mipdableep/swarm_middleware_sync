#include "../../include/swarm_middleware_api/node.hpp"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <sstream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <thread>

int rc_x,rc_y,rc_z,rc_Rz;
int x = 0,y = 0,z = 0,Rz = 0;

int messages_hrz = 10;
bool vel_recived = false;

void update_location(const ros_alate::Msg_Yaml &vel_in)
{
    std::stringstream ss(vel_in);
    std::string line;

    while (getline(ss, line))
    {
        std::string key = line.substr(0, line.find(":"));
        float value = std::stof(line.substr(line.find(":") + 2));
        if (key == "linear_x")
            {rc_x = value / messages_hrz;}

        else if (key == "linear_y")
            {rc_y = value / messages_hrz;}

        else if (key == "linear_z")
            {rc_z = value / messages_hrz;}

        else if (key == "yaw")
            {rc_Rz = value / messages_hrz;}
    }
    
    std::cout << rc_x << "\t" << rc_y << "\t" << rc_z << "\t" << rc_Rz << std::endl;

    z += rc_z;
    Rz += rc_Rz;

    float rad_Rz = Rz * M_PI / 180.0;

    Eigen::Matrix<float, 2, 2> R;
    R << std::cos(rad_Rz), -std::sin(rad_Rz),
         std::sin(rad_Rz),  std::cos(rad_Rz);
    
    Eigen::Vector2f t;
    t << rc_x, rc_y;

    Eigen::Vector2f rt = R * t;
    x += rt[0];
    y += rt[1];
    
    std::cout << "x:"  << x  << "\t";
    std::cout << "y:"  << y  << "\t";
    std::cout << "z:"  << z  << "\t";
    std::cout << "Rz:" << Rz << "\n" << std::endl;

    vel_recived = true;
}

std::string make_vel_msg(int x, int y, int z, int Rz)
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

    bool br = false;

    auto UL = [&](std::string a)
    {

        update_location(a);

        auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};
        auto const INTERFACES = std::vector<std::string>{"swarm_interfaces", "alate_interfaces"};
        auto interface_type_vel = InterfaceType("alate_interfaces", "Velocity");

        std::stringstream msg;
        msg << "abort: false\ncamera_angles: " << Rz
        << "\ncamera_elevation: 0\nlocation: "
        << "\n\taltitude: "  << z
        << "\n\tlatitude: "  << y
        << "\n\tlongitude: " << x;

        ros_alate::InterfaceType interface_type_drone_alt = ros_alate::InterfaceType("swarm_interfaces", "DroneOrientationAndAttitude");
        ros_alate::QosSettings qos = ros_alate::QosSettings{ros_alate::ReliabilityQosEnum::BEST_EFFORT, 10};
        user_api.set_advertiser("location", interface_type_drone_alt, qos);
        user_api.advertise("location", msg.str());
    };

    user_api.listen("velocity_t", interface_type_vel, qos, UL);


    user_api.spin();
}