#include "../include/aruco.h"

#include <opencv2/core/hal/interface.h>
#include <unistd.h>

#include <opencv2/highgui.hpp>
#include <vector>



std::vector<cv::Mat> aruco::getCameraCalibration(const std::string &path) {
    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened())
        throw std::runtime_error(
            "CameraParameters::readFromXMLFile could not open file:" + path);
    int w = -1, h = -1;
    cv::Mat MCamera, MDist;

    fs["image_width"] >> w;
    fs["image_height"] >> h;
    fs["distortion_coefficients"] >> MDist;
    fs["camera_matrix"] >> MCamera;

    if (MCamera.cols == 0 || MCamera.rows == 0) {
        fs["Camera_Matrix"] >> MCamera;
        if (MCamera.cols == 0 || MCamera.rows == 0)
            throw cv::Exception(
                9007,
                "File :" + path + " does not contains valid camera matrix",
                "CameraParameters::readFromXML", __FILE__, __LINE__);
    }

    if (w == -1 || h == 0) {
        fs["image_Width"] >> w;
        fs["image_Height"] >> h;
        if (w == -1 || h == 0)
            throw cv::Exception(
                9007,
                "File :" + path + " does not contains valid camera dimensions",
                "CameraParameters::readFromXML", __FILE__, __LINE__);
    }
    if (MCamera.type() != CV_32FC1) MCamera.convertTo(MCamera, CV_32FC1);

    if (MDist.total() < 4) {
        fs["Distortion_Coefficients"] >> MDist;
        if (MDist.total() < 4)
            throw cv::Exception(
                9007,
                "File :" + path +
                    " does not contains valid distortion_coefficients",
                "CameraParameters::readFromXML", __FILE__, __LINE__);
    }
    MDist.convertTo(MDist, CV_32FC1);
    std::vector<cv::Mat> newCameraParams = {MCamera, MDist};
    return newCameraParams;
}

void aruco::getMarkerIds(){
    stop = false;
    std::cout << "started track thread" << std::endl;
    std::vector<std::vector<cv::Point2f>> corners;
    const std::vector<cv::Mat> cameraParams =
        getCameraCalibration(yamlCalibrationPath);
    const cv::Ptr<cv::aruco::Dictionary> dictionary =
        cv::aruco::getPredefinedDictionary(
            cv::aruco::DICT_ARUCO_ORIGINAL /*DICT_4X4_100*/);

    cv::Mat imageCopy;
    int frame_counter = 0;
    int counter = 0;
    std::string frame_name;

    while (!stop) {
        if (frame && !frame->empty()) {

            cv::aruco::detectMarkers(*frame, dictionary, corners, ids);
            (*frame).copyTo(imageCopy);

            // blur is contro by size of the block Size(x,y) of moving windows
            /*blur(*frame, imageCopy, cv::Size(3 ,3));
            medianBlur(*frame, imageCopy, 5);*/

            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
            cv::Mat temp(imageCopy);

            frame_name ="/home/pi/arucoSwarm/videocap/" + std::to_string(frame_counter) + ".jpg";
            cv::imwrite(frame_name, temp);
            frame_counter ++;
        
            
            if (imshowStream)
                cv::imshow("aruco", imageCopy);

            cv::waitKey(1);
        } else {
            std::cout << "no frames" << std::endl;
            usleep(100000);
            continue;
        }
        bool canContinue = true;

        // if at least one marker detected
        if (canContinue) {
            if (ids.empty()){
                arucoDetected = false;
                if (counter&10 == 0)
                std::cout << "didnt detect marker" << std::endl;
            } else
                arucoDetected = true;

        }  // end of canConteniue
        counter ++;
        usleep(100000);
    }
}

void aruco::getCameraFeed() {
    runCamera = true;
    int curr_image = 0;
    while (runCamera) {
        if (!capture || !(capture->isOpened()) || *holdCamera) {
            usleep(5000);
            continue;
        }
        cv::Mat temp_frame;
        capture->read(temp_frame);
        *frame = temp_frame.clone();

        std::vector<uchar> frame_vec;
        // Pushing frame to queue
        frame_vec.assign(
            temp_frame.data,
            temp_frame.data + temp_frame.total() * temp_frame.channels());

        frame_queue.push(frame_vec);
    }
}

aruco::aruco(std::string &yamlCalibrationPath, int cameraPort,
             float currentMarkerSize)
    : frame_queue(1) {
    this->yamlCalibrationPath = yamlCalibrationPath;
    stop = false;
    holdCamera = std::make_shared<bool>(false);
    frame = std::make_shared<cv::Mat>();
    capture = std::make_shared<cv::VideoCapture>();
    if (capture->open(cameraPort)) {
        std::cout << "camera opened" << std::endl;
        capture->set(3, 640);
        capture->set(4, 480);
    } else {
        std::cout << "couldnt open camera by port" << std::endl;
    }
    this->currentMarkerSize = currentMarkerSize;
    cameraThread = std::move(std::thread(&aruco::getCameraFeed, this));
    arucoThread = std::move(std::thread(&aruco::getMarkerIds, this));
}

aruco::aruco(std::string &yamlCalibrationPath, std::string &cameraString,
             float currentMarkerSize)
    : frame_queue(1) {
    this->yamlCalibrationPath = yamlCalibrationPath;
    stop = false;
    holdCamera = std::make_shared<bool>(false);
    frame = std::make_shared<cv::Mat>();
    capture = std::make_shared<cv::VideoCapture>();
    capture->open(cameraString);
    capture->set(3, 960);
    capture->set(4, 720);
    this->currentMarkerSize = currentMarkerSize;
    cameraThread = std::move(std::thread(&aruco::getCameraFeed, this));
    arucoThread = std::move(std::thread(&aruco::getMarkerIds, this));
}

aruco::~aruco() {
    stop = true;
    runCamera = false;
    cameraThread.join();
    arucoThread.join();
    capture->release();
}


boost::lockfree::spsc_queue<std::vector<uchar>> &aruco::get_frame_queue() {
    return frame_queue;
}
