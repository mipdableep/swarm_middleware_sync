#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include "../../include/swarm_middleware_api/node.hpp"
#include <iostream>
#include <unistd.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <memory>
#include <thread>


bool run_camera;
std::string camera_string;

std::shared_ptr<cv::Mat> frame;
std::shared_ptr<cv::VideoCapture> capture;

const std::vector<int> leagal_ids = {635, 11, 12};
std::vector<std::vector<cv::Point2f>> corners;
std::vector<int> ids;
const cv::Ptr<cv::aruco::Dictionary> dictionary = 
    cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
