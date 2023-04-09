#include <ctello.h>
#include <unistd.h>

#include "aruco.h"
#include "drone_client.hpp"

#include <cmath>
#include <deque>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include <vector>

void ScanForAruco(aruco& detector, int arucoId, bool& runDetection,
                  bool& canContinue);

void searchArucoTargetThread(ctello::Tello& tello, aruco& detector, int ArucoTarget);

void scan360(aruco& detector, int arucoId, ctello::Tello& tello);

void scanForward(aruco& detector, int arucoId, ctello::Tello& tello);

// declare vars
bool exitLoop = false;
int turn_amount;

// declare commands
std::string standStill = "rc 0 0 0 0";
std::string forward;

std::string wpa_supplicant_tello_file_path;
