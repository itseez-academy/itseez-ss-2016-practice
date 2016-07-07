#pragma once

#include <memory>
#include <string>

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"

class Tracker {
 public:
  static std::shared_ptr<Tracker> CreateTracker(const std::string &name);
  virtual bool Init(const cv::Mat &frame, const cv::Rect &roi) = 0;
  virtual cv::Rect Track(const cv::Mat &frame) = 0;
};

class MedianFlowTracker : Tracker {
public:
	static std::shared_ptr<MedianFlowTracker> CreateTracker(const std::string &name)
	{
		if (name == "median_flow") {
			return std::make_shared<MedianFlowTracker>();
		}
	};
	virtual bool Init(const cv::Mat &frame, const cv::Rect &roi);
	virtual cv::Rect Track(const cv::Mat &frame);

protected:
	cv::Rect position_;
	cv::Mat frame_;
};