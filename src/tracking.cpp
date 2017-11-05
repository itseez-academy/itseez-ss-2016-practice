#include "tracking.hpp"

#include "opencv2/imgproc.hpp"
#include "opencv2/video/tracking.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>

using std::string;
using std::vector;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  if (name == "median_flow") {
    return std::make_shared<MedianFlowTracker>();
  } else {
    throw std::invalid_argument("Failed to create tracker");
  }
  /*std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;*/
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi) {
  if (!frame.empty() && !roi.empty()) {
    frame_ = frame;
    position_ = roi;
    return true;
  }
  return false;
}

Rect MedianFlowTracker::Track(const cv::Mat &frame) {
  Mat obj = frame_(position_);
  vector<Point2f> corners;

  goodFeaturesToTrack(obj, corners, 100, 0.01, 5);
  if (corners.empty()) {
    std::cout << "object lost" << std::endl;
  }

  Mat nextFrame;
  frame.copyTo(nextFrame);
  vector<Point2f> nextCorners;
  vector<uchar> status;
  vector<float> err;

  calcOpticalFlowPyrLK(frame_, nextFrame, corners, nextCorners, status, err);

  for (int i = 0; i < status.size(); ++i) {
    if (status[i] == 0) {
      status.erase(status.begin() + i);
      corners.erase(corners.begin() + i);
      nextCorners.erase(nextCorners.begin() + i);
      err.erase(err.begin() + i);
    }
  }

  vector<float> shiftX, shiftY;
  for (int i = 0; i < corners.size(); ++i) {
    shiftX.push_back(fabs(nextCorners[i].x - corners[i].x));
    shiftY.push_back(fabs(nextCorners[i].y - corners[i].y));
  }
  std::sort(shiftX.begin(), shiftX.end());
  std::sort(shiftY.begin(), shiftY.end());

  Point2f median(0, 0);
  int m = corners.size() / 2;
  if (m == 0) {
    median.x = (shiftX[m - 1] + shiftX[m]) / 2;
    median.y = (shiftY[m - 1] + shiftY[m]) / 2;
  } else {
    median.x = shiftX[m];
    median.y = shiftY[m];
  }

  Rect nextPosition = position_ + Point(median);
  
  position_ = nextPosition;
  frame_ = nextFrame;

  return nextPosition;
}
