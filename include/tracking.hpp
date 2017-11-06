#pragma once

#include <memory>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"

class Tracker {
 public:
  static std::shared_ptr<Tracker> CreateTracker(const std::string &name);
  virtual bool Init(const cv::Mat &frame, const cv::Rect &roi) = 0;
  virtual cv::Rect Track(const cv::Mat &frame) = 0;
};

class MedianFlowTracker : public Tracker {
public:
  virtual bool Init(const cv::Mat &frame, const cv::Rect &roi);
  virtual cv::Rect Track(const cv::Mat &frame);

protected:
  cv::Rect position_;
  cv::Mat frame_;
  void eraseBad(std::vector<uchar> &status, std::vector<cv::Point2f> &corners, std::vector<cv::Point2f> &nextCorners, std::vector<float> &err);
  float Median(std::vector<float> &arr);
  float getCoeff(std::vector<cv::Point2f> &corners, std::vector<cv::Point2f> &next_corners);
};
