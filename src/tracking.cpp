#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using std::cerr;
using std::endl;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) 
{
	if (name == "median_flow") 
	{
		return std::make_shared<MedianFlowTracker>();
	}
	else
	{
		cerr << "Failed to create tracker with name '" << name << "'"<< endl;
		return nullptr;
	}
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{
	return false;
}

Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	return Rect();
}