#include "../include/swarm_middleware_api/node.hpp"
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

void print_msg_callback(const std::string &msg)
{
    std::cout << "Received message:\n"
              << msg << std::endl;
}

void update_location(const std::string &msg)
{
    int rc_x,rc_y,rc_z,rc_Rz;

    std::stringstream ss(msg);
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

void run(ros_alate::Node &user_api, bool &run_loop)
{
    using ros_alate::InterfaceType;
    using ros_alate::Node;
    using ros_alate::QosSettings;
    using ros_alate::ReliabilityQosEnum;

    auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};

    auto interface_type_vel = InterfaceType("alate_interfaces", "Velocity");
    auto interface_type_drone_alt = InterfaceType("swarm_interfaces", "DroneOrientationAndAttitude");

    bool br = false;

    while (run_loop)
    {
        user_api.listen("velocity_t", interface_type_vel, qos, update_location);
        while (!vel_recived)
        {
            user_api.spinOnce();
            usleep(10000);
            if (!run_loop){
                br = true;
                break;
            }
        }

        if (br)
            break;

        std::stringstream msg;
        msg << "abort: false\ncamera_angles: " << Rz
        << "\ncamera_elevation: 0\nlocation:\n"
        << "\taltitude: "  << z
        << "\tlatitude: "  << y
        << "\tlongitude: " << x;

        user_api.listen("drone_orientation_attitude_t", interface_type_drone_alt, qos, print_msg_callback);
        user_api.set_advertiser("drone_orientation_attitude_t", interface_type_drone_alt, qos);
        user_api.advertise("drone_orientation_attitude_t", msg.str());

        vel_recived = false;
    }
}

