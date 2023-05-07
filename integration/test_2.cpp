#include <iostream>
#include <unistd.h>
#include <cmath>
#include <sstream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <thread>

int x = 0,y = 0,z = 0,Rz = 0;

int messages_hrz = 1;
bool vel_recived = false;


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

std::string make_msg(int x, int y, int z, int Rz)
{
    return ("linear_x: " + std::to_string(x) + 
          "\nlinear_y: " + std::to_string(y) +
          "\nlinear_z: " + std::to_string(z) +
          "\npitch: 0.0\nroll: 0.0\nyaw: " + std::to_string(Rz));
}

int main()
{
    // update_location(make_msg(0,0,0,0));
    update_location(make_msg(1,0,0,0));
    update_location(make_msg(1,0,0,0));
    update_location(make_msg(1,0,0,90));
    update_location(make_msg(1,0,0,0));
}