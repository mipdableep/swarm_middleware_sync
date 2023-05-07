#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include "../include/swarm_middleware_api/node.hpp"
#include <iostream>
#include <unistd.h>


const std::vector<int> leagal_ids = {10, 11, 12};
std::vector<std::vector<cv::Point2f>> corners;
std::vector<int> ids;
const cv::Ptr<cv::aruco::Dictionary> dictionary = 
    cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);

void print_msg_callback(const std::string &msg)
{
    std::cout << "Received message:\n"
              << msg << std::endl;
}

void detect_allys(cv::Mat img, ros_alate::Node &user_api)
{
    if (img.empty())
        std::cout<<"frame not recived properly"<<std::endl;

    cv::aruco::detectMarkers(img, dictionary, corners, ids);

    if (ids.empty())
        return;
    
    for (int i : ids)
    {
        if (!std::count(leagal_ids.begin(), leagal_ids.end(), i))
        {
            continue;
        }

        using ros_alate::InterfaceType;
        using ros_alate::Node;
        using ros_alate::QosSettings;
        using ros_alate::ReliabilityQosEnum;

        auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};
        std::stringstream msg;
        msg << "fail_id: " + std::to_string(i);
        
        auto interface_type = InterfaceType("swarm_interfaces", "ObstaclesAndDrones");
        user_api.listen("peer_detection", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("peer_detection", interface_type, qos);
        user_api.advertise("peer_detection", msg.str());
        user_api.spinOnce();
    }
    
}