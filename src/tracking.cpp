#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;
using namespace std;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  if (name == "median_flow") {
	  return std::make_shared<MedianFlowTracker>();
  }
  return nullptr;
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{
	frame_ = frame;
	position_ = roi;
	return true;
}

cv::Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	const int maxCorners = 100;
	const double qualityLevel = 0.01;
	const double minDistance = 10;
	
	//cout << "Conversion" << endl;
	Mat frameGray, nextFrameGray;
	cv::cvtColor(frame_, frameGray, CV_RGB2GRAY);
	cv::cvtColor(frame, nextFrameGray, CV_RGB2GRAY);
	frameGray.convertTo(frameGray, CV_8UC1);
	nextFrameGray.convertTo(nextFrameGray, CV_8UC1);
	Mat frameGrayRoi = frameGray(position_);

	//cout << "Features" << endl;
	std::vector< cv::Point2f > points;
	cv::goodFeaturesToTrack(frameGrayRoi, points, maxCorners, qualityLevel, minDistance);
	for (int i = 0; i < points.size(); i++)
	{
		points[i].x += position_.x;
		points[i].y += position_.y;
	}

	//cout << "Optical flow" << endl;
	std::vector< cv::Point2f > pointsNext;
	std::vector<float> err;
	std::vector<uchar> featuresFound;
	calcOpticalFlowPyrLK(frameGray, nextFrameGray, points, pointsNext, featuresFound, err);
	
	//cout << "Bbox restore" << endl;
	std::vector< cv::Point2f > good_new;
	std::vector< cv::Point2f > good_old,snew,sold;
	std::vector< float > newx, newy, oldx, oldy;
	std::vector< float >  smx, smy, smxsort, smysort;
	for (int i = 0; i < featuresFound.size();i++)
		if (featuresFound.at(i) != 0)
		{
			good_new.push_back(pointsNext.at(i));
			good_old.push_back(points.at(i));
		}
	cout << featuresFound.size();
	for (int i = 0; i < good_new.size(); i++)
	{
		smx.push_back(good_new.at(i).x - good_old.at(i).x);
		smy.push_back(good_new.at(i).y - good_old.at(i).y);
		//oldx.push_back(good_new.at(i).x);
		//oldy.push_back(good_new.at(i).y);
	}
	//cout << 5;
	std::sort(smx.begin(), smx.end());
	std::sort(smy.begin(), smy.end());
	//cout << 6;
	float medianx, mediany;
	medianx = smx.at(smx.size() / 2);
	mediany = smy.at(smy.size() / 2);
	cv::Rect NewPosition = position_;
	cout << medianx<<endl;
	cout << mediany << endl;
	NewPosition.x = position_.x + medianx;
	NewPosition.y = position_.y + mediany;
	//cout << "x:" << NewPosition.x <<endl;
	//cout << "y:" << NewPosition.y <<endl;
	//Last Move Of Code
	//position_ = NewPosition;


	return NewPosition;
}