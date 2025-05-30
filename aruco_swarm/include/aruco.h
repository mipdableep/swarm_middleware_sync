//
// Created by tzuk on 6/6/22.
//

#ifndef INC_2022SUMMERCOURSE_ARUCO_H
#define INC_2022SUMMERCOURSE_ARUCO_H

#include <boost/lockfree/spsc_queue.hpp>
#include <iostream>
#include <memory>
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

#include "../include/swarm_middleware_api/node.hpp"

class aruco
{
public:
      aruco(std::string &yamlCalibrationPath, int cameraPort,
      float currentMarkerSize, ros_alate::Node &user_api);

      ~aruco();

      aruco(std::string &yamlCalibrationPath, std::string &cameraString,
      float currentMarkerSize, ros_alate::Node &user_api);

      void getMarkerIds(ros_alate::Node &user_api);

      double forwardDistance(std::vector<cv::Vec3d> localRvecs,
           std::vector<cv::Vec3d> localTvecs, int Id);

      void set_dr_id(int id1, int id2, int id3);

      std::pair<int, int> twoClosest(std::vector<cv::Vec3d> localRvecs,
       std::vector<cv::Vec3d> localTvecs);
      boost::lockfree::spsc_queue<std::vector<uchar>> &get_frame_queue();

      int ID = -1;
      bool init = true;
      bool inFormation = false;
      int rightInForm = 1;
      bool arucoDetected = false;
      bool imshowStream;
      std::vector<int> ids;
      double upDown = 0.0;
      double forward = 0.0;
      double rightLeft = 0.0;
      std::pair<int, bool> leftOverAngle{0, false};
      int yaw = 0;
      int rollAngle = 0;


private:
      bool runCamera;
      bool stop;
      std::string yamlCalibrationPath;
      std::shared_ptr<cv::Mat> frame;
      float currentMarkerSize;
      std::thread cameraThread;
      std::thread arucoThread;
      std::shared_ptr<bool> holdCamera;
      std::shared_ptr<cv::VideoCapture> capture;
      int dr_id_1, dr_id_2, dr_id_3;

      boost::lockfree::spsc_queue<std::vector<uchar>> frame_queue;

      long amountOfUSleepForTrackMarker = 5000;

      std::vector<cv::Mat> getCameraCalibration(const std::string &path);

      void getCameraFeed();
};

#endif // INC_2022SUMMERCOURSE_ARUCO_H
