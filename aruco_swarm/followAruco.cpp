#include "./include/followArcuo.h"

using namespace std::chrono_literals;

void print_msg_callback_2(const std::string &msg)
{
    std::cout << "Received message:\n"
              << msg << std::endl;
}

void webcamTest(aruco& detector) {
    std::cout<< "in: webcamTest()" << std::endl;
    int tmpId = -1;
    bool commandFlag;

    while (true) {
        if (!detector.init || detector.ID != -1) {
        } else {
        }

        commandFlag = false;
    }
    sleep(1);
}

void followAruco(aruco& detector, ctello::Tello& tello, int ArucoFront, int ArucoBack, int ArucoTarget) {

    std::cout<< "in: followAruco()" << std::endl;

    bool isFirst = false;
    if (ArucoFront == -1) isFirst = true;
    
    tello.SendCommand(standStill);
    sleep(1);
    tello.SendCommand(standStill);

    if (isFirst)
        scan360(detector, ArucoBack, tello);
    else{
        scan360(detector, ArucoFront, tello);
    }


    std::thread detectAruco(
        [&] { searchArucoTargetThread(tello, detector, ArucoTarget); });
 
    while (!exitLoop) {
        tello.SendCommand(forward);
        sleep(4);

        scanForward(detector, ArucoFront, tello);

        scan360(detector, ArucoBack, tello);

        scanForward(detector, ArucoFront, tello);
    }
}

void searchArucoTargetThread(ctello::Tello& tello, aruco& detector, int ArucoTarget){
    std::cout<< "in: searchArucoTargetThread()" << std::endl;
    
    bool runDetection = true;
    bool targetDetected = false;
    int counter = 0;
    while (runDetection) {
        for (int i : detector.ids) {
            if (i == ArucoTarget) counter++;
        }
        usleep(70000);
        if (counter > 2){
            runDetection = false;
            targetDetected = true;
        }
    }

    if (targetDetected) {
        std::cout << "landing, object detected" << std::endl;
        
        tello.SendCommand("down 200");
        sleep(3);
        tello.SendCommand("rc 0 0 0 90");
        sleep(3);
        tello.SendCommand("rc 0 0 0 90");
        sleep(3);
        tello.SendCommandWithResponse("land");
        //for green led
        exit(3);
    
    } else {
        usleep(70000);
    }
}



void scan360(aruco& detector, int arucoId, ctello::Tello& tello){
    std::cout<< "in: scan360()" << std::endl;
    std::cout << "searching for aruco " << arucoId << std::endl;

    bool runDetection = true;
    bool canContinue;
    std::thread detectAruco(
        [&] { ScanForAruco(detector, arucoId, runDetection, canContinue); });

    std::string turnCommand = "cw " + std::to_string(360/turn_amount);

    for (int i = 0; i<turn_amount; i++){
        tello.SendCommand(turnCommand);
        usleep(4000000);
    }
    tello.SendCommand(standStill);
    runDetection = false;
    usleep(300000);

    detectAruco.join();

    if (!canContinue && arucoId != -1) {
        std::cout << "didn't detect aruco " << arucoId << ", landing!"
                  << std::endl;

        tello.SendCommandWithResponse("land");
        sleep(4);

        //for red led
        exit(2);
    }
}

void scanForward(aruco& detector, int arucoId, ctello::Tello& tello){
    std::cout<< "in: scanForward()" << std::endl;
    std::cout << "searching for aruco " << arucoId << std::endl;

    bool runDetection = true;
    bool canContinue;
    std::thread detectAruco(
        [&] { ScanForAruco(detector, arucoId, runDetection, canContinue); });

    tello.SendCommand(standStill);
    usleep(2000000);
    tello.SendCommand("cw 60");
    usleep(4000000);
    tello.SendCommand("ccw 120");
    usleep(4000000);
    tello.SendCommand("cw 60");
    usleep(4000000);


    tello.SendCommand(standStill);
    runDetection = false;
    usleep(1000000);

    detectAruco.join();

    if (!canContinue && arucoId != -1) {
        std::cout << "didn't detect aruco " << arucoId << ", landing!"
                  << std::endl;
        
        tello.SendCommandWithResponse("land");
        sleep(4);
        //for red led
        exit(2);
    }
}

void ScanForAruco(aruco& detector, int arucoId, bool& runDetection, bool& canContinue) {
    std::cout<< "in: ScanForAruco()" << std::endl;
    int counter = 0;
    while (runDetection) {
        for (int i : detector.ids) {
            if (i == arucoId) counter++;
            std::cout<<"++ "<<std::endl;
        }
        usleep(100000);
    }
    std::cout << std::endl << "aruco counter = " << counter << std::endl;

    if (counter > 3 || arucoId == -1)
        canContinue = true;
    else {
        canContinue = false;
        std::cout << "aruco wasn't detected enough" << std::endl;
    }
}

void change_to_tello_wifi() {
    std::cout<< "in: change_to_tello_wifi()" << std::endl;
    
    std::string kill_connection_cmd = "sudo killall wpa_supplicant";
    std::string connection_cmd =
        "sudo wpa_supplicant -i wlan0 -B -c " + wpa_supplicant_tello_file_path;

    std::ofstream fifo;

    fifo.open(PIPE_path.c_str());
    if (!fifo){std::cerr << "Failed to open named pipe" << std::endl; exit(EXIT_FAILURE);}
    fifo << kill_connection_cmd << std::endl;
    fifo.close();
    std::this_thread::sleep_for(2s);

    fifo.open(PIPE_path.c_str());
    if (!fifo){std::cerr << "Failed to open named pipe" << std::endl; exit(EXIT_FAILURE);}
    fifo << connection_cmd << std::endl;
    fifo.close();
    std::this_thread::sleep_for(10s);
}

auto main(int argc, char **argv) -> int
{

    
    std::ifstream config("../config.json");

    nlohmann::json conf;
    config >> conf;
    config.close();

    // global conf str
    std::string G = "global-config";

    //* drone number - for drone-config
    std::string DEVICE = conf[G]["DEVICE"];
    
    // *PIPE
    PIPE_path = conf[G]["PIPE_path"];
    // run settings
    bool send_takeoff  = conf[G]["send_takeoff"];

    // run vars
    int dist_forward   = conf[G]["dist_forward"];
    int land_target_id = conf[G]["land_target_id"];

    float currentMarkerSize = conf[G]["currentMarkerSize"];

    turn_amount = conf[G]["turn_amount"];
    forward     = "forward " + std::to_string(dist_forward);

    int aruco_forward_id = conf[DEVICE]["aruco_forward_id"];
    int aruco_back_id    = conf[DEVICE]["aruco_back_id"];
    int aruco_self_id    = conf[DEVICE]["aruco_self_id"];

    // capture settings
    bool isWebcam  = DEVICE == "webcam";
    int cameraPort = conf[G]["cameraPort"];

    std::string cameraString = conf[G]["cameraString"];

    // video settings
    bool do_imshow  = conf[G]["imshow"];
    bool save_video = conf[G]["save_video"];
    std::string save_video_path = conf[G]["save_video_path"];

    // connection settings
    bool runServer            = conf[G]["runServer"];
    int serverPort            = conf[G]["serverPort"];
    std::string serverHostIp  = conf[G]["serverHostIp"];
    bool rpi_connect_to_drone = conf[G]["rpi_connect_to_drone"];
    bool do_nmcli_command      = conf[G]["do_nmcli_command"];

    // calibration settings
    std::string yamlCalibrationPath = conf[DEVICE]["calibration_path"];
    std::string droneName           = conf[DEVICE]["DroneName"];
    
    // tello conf path
    wpa_supplicant_tello_file_path  = conf[DEVICE]["tello_conf_path"];

    using ros_alate::InterfaceType;
    using ros_alate::Node;
    using ros_alate::QosSettings;
    using ros_alate::ReliabilityQosEnum;

    auto const NODE_NAME = std::string("ros_alate_middeware_demo");
    auto const INTERFACES = std::vector<std::string>{"swarm_interfaces", "alate_interfaces"};
    auto user_api = Node(argc, argv, NODE_NAME, INTERFACES);

    // !start of tests!
    // !----------------------------------------------------------------------------
    
    auto qos = QosSettings{ReliabilityQosEnum::BEST_EFFORT, 10};
    std::stringstream msg;
    msg << "fail_id: " + std::to_string(58);
    auto interface_type = InterfaceType("swarm_interfaces", "ObstaclesAndDrones");
    user_api.listen("obstacles_drones_t", interface_type, qos, print_msg_callback_2);
    user_api.set_advertiser("obstacles_drones_t", interface_type, qos);
    user_api.advertise("obstacles_drones_t", msg.str());
    user_api.spinOnce();

    if (false)
    {
        aruco detector(yamlCalibrationPath, cameraPort, currentMarkerSize, user_api);
        detector.imshowStream = do_imshow;

        std::thread movementThread
        ([&]{
            webcamTest(detector);
        });
    }

    // !start of drone part!
    // !----------------------------------------------------------------------------
    if (runServer){
        DroneClient client(droneName, serverHostIp, serverPort);
        client.connect_to_server();
        client.wait_for_takeoff();
        change_to_tello_wifi();
    }

    if (!runServer && rpi_connect_to_drone)
        change_to_tello_wifi();

    if (do_nmcli_command)
        std::system(((std::string)"nmcli c up TELLO-9EE78B").c_str());

    ctello::Tello tello;

    tello.SendCommandWithResponse("streamon");

    sleep(2);

    if (send_takeoff){
        tello.SendCommandWithResponse("takeoff");
        tello.SendCommand("rc 0 0 0 0");
        sleep(2);
        tello.SendCommand("up 70");
    }

    aruco detector(yamlCalibrationPath, cameraString, currentMarkerSize, user_api);
    detector.imshowStream = do_imshow;
    detector.set_dr_id(aruco_forward_id, aruco_back_id, aruco_self_id);
    // Detector object_detector(argv[1], detector.get_frame_queue());
    std::thread movementThread
    ([&]{
        followAruco(detector, tello, aruco_forward_id, aruco_back_id, land_target_id); 
        });

    movementThread.join();
    
}