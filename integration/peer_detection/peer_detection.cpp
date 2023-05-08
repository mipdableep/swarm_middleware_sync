#include "peer_detection.hpp"

void print_msg_callback(const std::string &msg)
{
    std::cout << "Received message:\n"
              << msg << std::endl;
}


void detect_allys(ros_alate::Node &user_api)
{
    while (true){
        usleep(5000);
        if (frame->empty())
        {
            continue;
        }


        cv::aruco::detectMarkers(*frame, dictionary, corners, ids);

        if (ids.empty())
            return;
        int counter = 0;
        for (int i : ids)
        {
            if (!std::count(leagal_ids.begin(), leagal_ids.end(), i))
            {
                continue;
            }

            counter ++;
            using ros_alate::InterfaceType;
            using ros_alate::Node;
            using ros_alate::QosSettings;
            using ros_alate::ReliabilityQosEnum;

            std::cout << "detected: ID " << i << std::endl;

            auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};
            std::stringstream msg;
            msg << "fail_id: " + std::to_string(i);
            
            auto interface_type = InterfaceType("swarm_interfaces", "ObstaclesAndDrones");
            user_api.listen("peer_detection", interface_type, qos, print_msg_callback);
            user_api.set_advertiser("peer_detection", interface_type, qos);
            user_api.advertise("peer_detection", msg.str());
            user_api.spinOnce();
        }
        if (counter == 0)
        {
            using ros_alate::InterfaceType;
            using ros_alate::Node;
            using ros_alate::QosSettings;
            using ros_alate::ReliabilityQosEnum;

            auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};
            
            auto interface_type = InterfaceType("swarm_interfaces", "ObstaclesAndDrones");
            user_api.listen("peer_detection", interface_type, qos, print_msg_callback);
            user_api.set_advertiser("peer_detection", interface_type, qos);
            user_api.advertise("peer_detection", "fail_id: 0");
            user_api.spinOnce();
        }
        std::cout << std::endl;
    }
}

void get_camera_feed()
{

    frame = std::make_shared<cv::Mat>();
    capture = std::make_shared<cv::VideoCapture>();
    capture->open(camera_string);
    capture->set(3, 960);
    capture->set(4, 720);

    run_camera = true;

    while (run_camera) {
        if (!capture || !(capture->isOpened())) {
            usleep(5000);
            continue;
        }
        cv::Mat temp_frame;
        capture->read(temp_frame);
        *frame = temp_frame.clone();
    }
}


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <drone ip>" << std::endl;
        return 1;
    }
    std::string drone_ip = argv[1];
    std::cout << "drone ip: " << drone_ip <<std::endl;
    camera_string = "udp://" + drone_ip + ":11111?overrun_nonfatal=1&fifo_size=5000";

    using ros_alate::InterfaceType;
    using ros_alate::Node;
    using ros_alate::QosSettings;
    using ros_alate::ReliabilityQosEnum;
 
    auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};

    auto const NODE_NAME = std::string("ros_alate_middeware_demo");
    auto const INTERFACES = std::vector<std::string>{"swarm_interfaces", "alate_interfaces"};
    auto user_api = Node(argc, argv, NODE_NAME, INTERFACES);

    std::thread peer_det_thread ([&] {detect_allys(user_api);});
    get_camera_feed();

    // std::thread camera_thread([&] {get_camera_feed();});
}