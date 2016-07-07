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
	frame.copyTo(frame_);
	position_ = roi;
	if (frame_.empty())
		return false;
	return true;
};

cv::Rect MedianFlowTracker::Track(const cv::Mat &frame) {
	std::vector<cv::Point2f> prevpos,nextpos;
	std::vector<uchar>        status;
	std::vector<float>         err;
	
	cv::Mat frame_roi;
	frame_(position_).copyTo(frame_roi);

	cv::goodFeaturesToTrack(frame_roi, prevpos, 50, 0.01, 10);

	for (size_t i = 0; i < prevpos.size(); i++)
	{
		prevpos[i].x += position_.x;
		prevpos[i].y += position_.y;
	}

	calcOpticalFlowPyrLK(frame_, frame, prevpos, nextpos, status, err);

	

	for (size_t i = 0; i < status.size(); i++)
	{
		if (status[i] == 0)
			prevpos.erase(prevpos.begin() + i);
	}
	
	calcOpticalFlowPyrLK(frame, frame_, nextpos, prevpos, status, err);
	for (size_t i = 0; i < status.size(); i++)
	{
		if (status[i] == 0)
			nextpos.erase(nextpos.begin() + i);
	}
	
	std::vector<int> X, Y;
	for (size_t i = 0; i < nextpos.size() && i<prevpos.size(); i++)
	{

		
		X.push_back((int)nextpos[i].x - (int)prevpos[i].x);
		Y.push_back((int)nextpos[i].y - (int)prevpos[i].y);
	}
	std::sort(X.begin(), X.end());
	std::sort(Y.begin(), Y.end());
	int dx = X[X.size() / 2];
	int dy = Y[Y.size() / 2];
	std::cout << "dx= " << dx<<"      " << "dy= " << dy<<std::endl;
	return Rect(position_.x+dx,position_.y+dy,position_.width,position_.height);
};