#include "location.cpp"
#include "ally_detection.cpp"


void print_msg_callback(const std::string &msg)
{
    std::cout << "Received message:\n"
              << msg << std::endl;
}

void main(int argc, char **argv)
{
    using ros_alate::InterfaceType;
    using ros_alate::Node;
    using ros_alate::QosSettings;
    using ros_alate::ReliabilityQosEnum;

    auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};

    auto const NODE_NAME = std::string("ros_alate_middeware");
    auto const INTERFACES = std::vector<std::string>{"swarm_interfaces", "alate_interfaces"};
    auto user_api = Node(argc, argv, NODE_NAME, INTERFACES);

    auto const location_node_name = std::string("ros_alate_location");
    auto location_node = Node(argc, argv, location_node_name, INTERFACES);

    bool run_loop = true;

    std::thread location_thread ([&] {run(location_node, run_loop);});
    
    std::string vel1 = "linear_x: 0.0/nlinear_y: 0.0/nlinear_z: 0.0/npitch: 0.0/nroll: 0.0/nyaw: 0.0";
    std::string vel2 = "linear_x: 10.0/nlinear_y: 0.0/nlinear_z: 0.0/npitch: 0.0/nroll: 0.0/nyaw: 0.0";
    std::string vel3 = "linear_x: 0.0/nlinear_y: 10.0/nlinear_z: 0.0/npitch: 0.0/nroll: 0.0/nyaw: 0.0";
    std::string vel4 = "linear_x: 0.0/nlinear_y: 0.0/nlinear_z: 10.0/npitch: 0.0/nroll: 0.0/nyaw: 0.0";

    auto interface_type_vel = InterfaceType("alate_interfaces", "Velocity");
    user_api.listen("velocity_t", interface_type_vel, qos, print_msg_callback);
    user_api.set_advertiser("velocity_t", interface_type_vel, qos);
    user_api.advertise("velocity_t", vel1);

    run_loop = false;
    sleep(1);
    location_thread.join();

}