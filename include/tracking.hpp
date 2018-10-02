#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"

class Tracker {
 public:
  static std::shared_ptr<Tracker> CreateTracker(const std::string &name);
  virtual bool Init(const cv::Mat &frame, const cv::Rect &roi) = 0;
  virtual cv::Rect Track(const cv::Mat &frame) = 0;
};


class MedianFlowTracker : public Tracker{
 private:
	double medianFunc(std::vector<double>);
	void delElements(std::initializer_list < std::vector<cv::Point2f>*>, std::vector<uchar>* status = nullptr, std::vector<double>* revErr = nullptr, double value = 0);
 public:
  bool Init(const cv::Mat &frame, const cv::Rect &roi) override;
  cv::Rect Track(const cv::Mat &frame) override;
  cv::Rect getRect() { return position_; }
  

 protected:
  cv::Rect position_;
  cv::Mat frame_;
};