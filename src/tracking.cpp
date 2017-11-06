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
    cvtColor(frame, frame_, CV_BGR2GRAY);
    position_ = roi;
    return true;
  }
  return false;
}

void MedianFlowTracker::eraseBad(vector<uchar> &status, vector<cv::Point2f> &corners, vector<cv::Point2f> &nextCorners, vector<float> &err) {
  for (int i = 0; i < status.size(); ++i) {
    if (status[i] == 0) {
      status.erase(status.begin() + i);
      corners.erase(corners.begin() + i);
      nextCorners.erase(nextCorners.begin() + i);
      err.erase(err.begin() + i);
    }
  }

  if (corners.empty()) throw std::logic_error("Not enough points");
}

float MedianFlowTracker::Median(std::vector<float> &arr) {
  std::sort(arr.begin(), arr.end());

  if (arr.size() % 2 == 0) {
    return (arr[(arr.size() / 2) - 1] + arr[arr.size() / 2])/2;
  } else {
    return arr[arr.size() / 2];
  }
}

float MedianFlowTracker::getCoeff(std::vector<cv::Point2f> &corners, std::vector<cv::Point2f> &next_corners) {
  vector<float> coeff;

  for (int i = 0; i < corners.size(); ++i) {
    for (int j = i + 1; j < corners.size() - 1; ++j) {
      float d_new = sqrt(pow((next_corners[i].x - next_corners[j].x), 2) + pow((next_corners[i].y - next_corners[j].y), 2));
      float d_old = sqrt(pow((corners[i].x - corners[j].x), 2) + pow((corners[i].y - corners[j].y), 2));
      coeff.push_back(d_new/d_old);
    }
  }
  return Median(coeff);
}

Rect MedianFlowTracker::Track(const cv::Mat &frame) {
  Mat obj = frame_(position_);
  vector<Point2f> corners;

  goodFeaturesToTrack(obj, corners, 100, 0.01, 5);
  if (corners.empty()) {
    std::cout << "object lost" << std::endl;
  }

  Mat next_frame;
  cvtColor(frame, next_frame, CV_BGR2GRAY);
  vector<Point2f> next_corners;
  vector<uchar> status;
  vector<float> err;

  calcOpticalFlowPyrLK(frame_, next_frame, corners, next_corners, status, err);
  eraseBad(status, corners, next_corners, err);

  calcOpticalFlowPyrLK(next_frame, frame_, next_corners, corners, status, err);
  eraseBad(status, corners, next_corners, err);

  float median_err = Median(err);
  for (int i = 0; i < err.size(); ++i) {
    if (err[i] > median_err) {
      status.erase(status.begin() + i);
      corners.erase(corners.begin() + i);
      next_corners.erase(next_corners.begin() + i);
      err.erase(err.begin() + i);
    }
  }
  if (corners.empty()) throw std::logic_error("Not enough points");

  vector<float> shiftX, shiftY;
  for (int i = 0; i < corners.size(); ++i) {
    shiftX.push_back(fabs(next_corners[i].x - corners[i].x));
    shiftY.push_back(fabs(next_corners[i].y - corners[i].y));
  }
  Point2f median_shift(Median(shiftX), Median(shiftY));
  float coeff = getCoeff(next_corners, corners);

  Rect next_position = position_ + Point(median_shift);
  next_position.width *= coeff;
  next_position.height *= coeff;
  
  position_ = next_position;
  frame_ = next_frame;

  return next_position;
}
