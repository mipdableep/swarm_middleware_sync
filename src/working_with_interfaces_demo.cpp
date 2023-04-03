#include <fstream>
#include <iostream>
#include <thread>

#include "swarm_middleware_api/node.hpp"
#include <yaml-cpp/yaml.h>

auto read_yaml_file(const std::string &file_path)
{
    // read file to string buffer
    std::ifstream file(file_path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void print_msg_callback(const std::string &msg)
{
    std::cout << "Received message:\n"
              << msg << std::endl;
}

auto main(int argc, char **argv) -> int
{
    using ros_alate::InterfaceType;
    using ros_alate::Node;
    using ros_alate::QosSettings;
    using ros_alate::ReliabilityQosEnum;
    // For example /home/pavelvm/dev/ros_alate/swarm_middleware_api/examples/
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <path_to_example_dir>" << std::endl;
        return 1;
    }

    std::cout << "swarm interfaces topic demo:\n";
    // create user_api object
    // node_name is used to create middleware node
    // interfaces is used to load shared libraries
    auto const NODE_NAME = std::string("ros_alate_middeware_demo");
    auto const INTERFACES = std::vector<std::string>{"swarm_interfaces", "alate_interfaces"};
    auto user_api = Node(argc, argv, NODE_NAME, INTERFACES);

    int const MAX_HISTORY = 10;
    auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, MAX_HISTORY};

    auto const EXAMPLE_DIR_PATH = std::string(argv[1]);

    { // Mission control enum topic
        auto mc_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("mission_control.yaml"));
        auto interface_type = InterfaceType("alate_interfaces", "McEnum");
        user_api.listen("mc_enum_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("mc_enum_t", interface_type, qos);
        user_api.advertise("mc_enum_t", mc_sample);
    }

    { // Remote command enum topic
        auto rc_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("remote_command.yaml"));
        auto interface_type = InterfaceType("alate_interfaces", "RcEnum");
        user_api.listen("rc_enum_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("rc_enum_t", interface_type, qos);
        user_api.advertise("rc_enum_t", rc_sample);
    }

    { // String topic
        auto string_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("string.yaml"));
        auto interface_type = InterfaceType("alate_interfaces", "String");
        user_api.listen("string_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("string_t", interface_type, qos);
        user_api.advertise("string_t", string_sample);
    }

    { // Velocity topic
        auto velocity_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("velocity.yaml"));
        auto interface_type = InterfaceType("alate_interfaces", "Velocity");
        user_api.listen("velocity_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("velocity_t", interface_type, qos);
        user_api.advertise("velocity_t", velocity_sample);
    }

    { // PlannedMission topic
        auto planned_mission_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("planned_mission.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "PlannedMission");
        user_api.listen("planned_mission_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("planned_mission_t", interface_type, qos);
        user_api.advertise("planned_mission_t", planned_mission_sample);
    }

    { // OperationalReport topic
        auto operational_report_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("operational_report.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "OperationalReport");
        user_api.listen("operational_report_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("operational_report_t", interface_type, qos);
        user_api.advertise("operational_report_t", operational_report_sample);
    }

    { // ObstaclesAndDrones topic
        auto obstacles_drones_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("obstacles_and_drone.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "ObstaclesAndDrones");
        user_api.listen("obstacles_drones_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("obstacles_drones_t", interface_type, qos);
        user_api.advertise("obstacles_drones_t", obstacles_drones_sample);
    }

    { // InitObstaclesAndDrones topic

        auto init_obstacles_drones_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("init_obstacles_drones.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "InitObstaclesAndDrones");
        user_api.listen("init_obstacles_drones_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("init_obstacles_drones_t", interface_type, qos);
        user_api.advertise("init_obstacles_drones_t", init_obstacles_drones_sample);
    }

    { // InitNavigation topic
        auto init_navigation_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("init_navigation.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "InitNavigate");
        user_api.listen("init_navigation_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("init_navigation_t", interface_type, qos);
        user_api.advertise("init_navigation_t", init_navigation_sample);
    }

    { // DroneOrientationAndAttitude topic
        auto drone_orientation_attitude_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("drone_orientation_attitude.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "DroneOrientationAndAttitude");
        user_api.listen("drone_orientation_attitude_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("drone_orientation_attitude_t", interface_type, qos);
        user_api.advertise("drone_orientation_attitude_t", drone_orientation_attitude_sample);
    }

    { // mission topic:
        auto mission_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("mission.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "Mission");
        user_api.listen("mission_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("mission_t", interface_type, qos);
        user_api.advertise("mission_t", mission_sample);
    }

    { // navigation topic:
        auto navigation_sample = read_yaml_file(EXAMPLE_DIR_PATH + std::string("navigation.yaml"));
        auto interface_type = InterfaceType("swarm_interfaces", "Navigation");
        user_api.listen("navigation_t", interface_type, qos, print_msg_callback);
        user_api.set_advertiser("navigation_t", interface_type, qos);
        for (int i = 0; i < 10; i++)
        {
            user_api.advertise("navigation_t", navigation_sample);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        // user_api.advertise("navigation_t", navigation_sample);
    }

    // use case for spinOnce() :
    int const NUM_OF_SPINS = 20;
    int const SLEEP_TIME_MS = 100;
    for (int i = 0; i < NUM_OF_SPINS; i++)
    {
        user_api.spinOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
    }

    // without  spin() the callbacks will not be called
    // will block until Ctrl+C

    return 0;
}
