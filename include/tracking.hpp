#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d.hpp"
#include"opencv2/imgproc.hpp"
#include"opencv2/video.hpp"
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
};