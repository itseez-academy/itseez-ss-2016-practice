#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  if (name == "median_flow") {
	  return std::make_shared<MedianFlowTracker>();
  }
  return nullptr;
}
bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi) {
	position_ = roi;
	frame_ = frame.clone();
	return true;
}
cv::Rect MedianFlowTracker::Track(const cv::Mat &frame) {
	std::vector<Point2f> features;
	std::vector<Point2f> features_;
	std::vector<uchar> status;
	std::vector<float> err;

	Mat frame_prev_roi(frame_, position_);

	goodFeaturesToTrack(frame_prev_roi, features, 100, 0.3, 5);
	goodFeaturesToTrack(frame, features_, 100, 0.3, 5);
	
	calcOpticalFlowPyrLK(frame_, frame, features_, features, status, err);

	int k = 0;
	for (int i = 0; i < status.size();i++)
		if (status[i] == 0)
		{
			features_.erase(features_.begin() + i - k);
			features.erase(features.begin() + i - k);
			k++;
		}

	std::vector<float>dx, dy;
	for (int i = 0; i < features_.size(); i++) {
		dx.push_back(features[i].x-features_[i].x);
		dy.push_back(features[i].y - features_[i].y);
	}

	std::sort(dx.begin(), dx.end());
	std::sort(dy.begin(), dy.end());
	position_.x += dx.size() / 2;
	position_.y += dy.size() / 2;

	return position_;
}
