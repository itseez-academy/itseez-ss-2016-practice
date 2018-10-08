#include "tracking.hpp"

#include <iostream>
#include <algorithm>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) 
{	
	if (name == "median_flow")
	{
		return std::make_shared<MedianFlowTracker>();
	}
	else
	{
		std::cerr << "Failed to create tracker with name '" << name << "'"
			<< std::endl;
		return nullptr;
	}
		
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{
	if (frame.empty()) return false;
	if (roi.empty()) return false;
	frame_ = frame;
	position_ = roi;
	return true;
}

cv::Rect MedianFlowTracker::Track(const cv::Mat & frame)
{	
	//init 
	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
	Size subPixWinSize(10, 10), winSize(31, 31);
	Mat tmp, tmp_gray, next_gray, prev_gray;
	std::vector<Point2f> features_prev, features_next; // Mat?
	std::vector<uchar> status;
	std::vector<float> err;
	frame_(position_).copyTo(tmp);

	// find features
	cvtColor(tmp, tmp_gray, COLOR_BGR2GRAY);
	goodFeaturesToTrack(tmp_gray, features_prev, -1, 0.01, 10);
	assert(features_prev.size() > 0);

	//calc optical flow
	cvtColor(frame_, prev_gray, COLOR_BGR2GRAY);
	cvtColor(frame, next_gray, COLOR_BGR2GRAY);
	calcOpticalFlowPyrLK(prev_gray, next_gray, features_prev, features_next, status, err,
		winSize, 3, termcrit, 0, 0.001);

	//filtration bad points
	
	size_t i, k; // counters

	for( i = k = 0; i < features_next.size(); i++ )
	{
		if (!status[i])
			continue;
		features_prev[k++] = features_prev[i];
		features_next[k++] = features_next[i];
		
	}
	features_prev.resize(k); // new points
    features_next.resize(k);
	
	//find median X and Y offsets
	std::vector<float> offsetX;
	std::vector <float> offsetY;
	Point2f newObjCenter;
	for (i = 0; i < features_next.size(); i++)
	{
		offsetX[i] = features_next[i].x - features_prev[i].x;
		offsetY[i] = features_next[i].y - features_prev[i].y;
	}
	
	float medianX , medianY;
	
	std::sort(offsetX.begin(), offsetX.end());
	std::sort(offsetY.begin(), offsetY.end());

	medianX = (int)offsetX[offsetX.size()/2];
	medianY = (int)offsetY[offsetY.size()/2]; 

	// new rect position
	Rect newPos(medianX - position_.width / 2, medianY - position_.height / 2, position_.width, position_.height);
	
	return newPos;
}
