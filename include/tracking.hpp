#pragma once

#include <memory>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2\opencv.hpp"

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
/*
struct MouseCallbackState {
	bool is_selection_started;
	bool is_selection_finished;
	Point point_first;
	Point point_second;
};*/