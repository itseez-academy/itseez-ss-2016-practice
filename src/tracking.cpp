#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
    if (name == "median_flow") {
        return std::make_shared<MedianFlowTracker>();
    }
  return nullptr;
}
bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{
	if (!frame.empty())return false;
	cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
	this->frame_ = frame;
	this->position_ = roi;
	return true;
}
cv::Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	cv::Mat prevPtr,nextPtr,err;
	std::vector<int> status;
	cv::goodFeaturesToTrack(frame_(position_), prevPtr, 50, 0.01, 10);
	cv::calcOpticalFlowPyrLK(frame_, frame, prevPtr, nextPtr, status, err);
	std::vector<cv::Point2f> _prevPtr(prevPtr), _nextPtr(nextPtr),_err(err);
	for (int i = 0; i < status.size(); i++) 
	{
		if (!status[0]) 
		{
			_prevPtr.erase(_prevPtr.begin() + i + 1);
			_nextPtr.erase(_nextPtr.begin() + i + 1);
			_err.erase(_err.begin() + i + 1);
		}
	}
	std::vector<float> x_shifts;
	std::vector<float> y_shifts;
	for (int i = 0; i < status.size(); i++)
	{
		x_shifts.push_back(_nextPtr[i].x - _prevPtr[i].x);
		y_shifts.push_back(_nextPtr[i].y - _prevPtr[i].y);
	
	}
	std::nth_element(x_shifts.begin(), x_shifts.begin() + status.size() / 2, x_shifts.end());
	std::nth_element(y_shifts.begin(), y_shifts.begin() + status.size() / 2, y_shifts.end());
	position_.x += x_shifts[status.size() / 2];
	position_.y += y_shifts[status.size() / 2];
	return cv::Rect(position_);
	
}