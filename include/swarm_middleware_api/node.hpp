#pragma once

#include <functional> // std::function
#include <string>     // std::string
#include <any>        // std::any
#include <vector>     // std::vector
namespace ros_alate
{
    using Msg_Yaml = std::string const;                        // yaml string
    using Topic = std::string const;                           // topic name
    using InterfaceType = std::pair<std::string, std::string>; // pair of package name and message name
    using Callback = std::function<void(Msg_Yaml &)>;          // callback function to call when a message is received. The callback function must have the following signature: void(const Msg_Yaml &a_msg)

    enum class ReliabilityQosEnum
    {
        BEST_EFFORT,
        RELIABLE
    };

    const int DEFAULT_HISTORY_DEPTH = 10;
    struct QosSettings
    {
        ReliabilityQosEnum m_reliability = ReliabilityQosEnum::BEST_EFFORT;
        int m_history_depth = DEFAULT_HISTORY_DEPTH;
    };

    class Node
    {
    public:
        /*
         * @brief constructor
         * @param argc number of arguments
         * @param argv arguments
         * @param node_name name of the middleware node
         * @param a_interfaces list of interfaces to load
         * @return void
         * @note argc and argv are used to initialize the node
         */
        Node(int argc, char **argv, std::string const &node_name, std::vector<std::string> const &a_interfaces);
        Node(Node const &) = delete;
        Node(Node &&) = delete;
        auto operator=(Node const &) -> Node & = delete;
        auto operator=(Node &&) -> Node & = delete;
        ~Node();

        /*
         * @brief advertise a message on a topic
         * @param a_topic topic to advertise on
         * @param a_msg_yaml message to advertise
         * @return void
         * @note a_msg_yaml must be a valid yaml string corresponding to the topic
         * @note see examples directory for examples
         */
        void advertise(Topic &a_topic, Msg_Yaml &a_msg_yaml);

        /*
         * @brief Set the advertiser for a topic
         * @param a_topic topic to advertise on
         * @param a_interface_type interface type to advertise
         * @param a_qos_settings QoS settings to use
         * @return void
         * @note a_interface_type must be a valid interface type
         */
        void set_advertiser(Topic &a_topic, InterfaceType &a_interface_type, QosSettings &a_qos_settings);

        /*
         * @brief listen to a topic
         * @param a_topic topic to listen to
         * @param a_interface_type interface type to listen to
         * @param a_qos_settings QoS settings to use
         * @param a_callback callback function to call when a message is received
         * @return void
         * @note a_callback must have the following signature: void(const Msg_Yaml &a_msg)
         * @note a_msg must be a valid yaml string and valid message for the topic
         */
        void listen(Topic &a_topic, InterfaceType &a_interface_type, QosSettings &a_qos_settings, Callback a_callback);

        /*
         * @brief spin the node
         * @return void
         * @note this function will block until the node is closed
         */
        void spin();

        /*
         * @brief spin the node once
         * @return void
         * @note this function will not block
         */
        void spinOnce();

    private:
        std::any impl_;
    };
} // namespace ros_alate