#include "tracking.hpp"

#include <iostream>
#include <algorithm>

using std::string;
using std::shared_ptr;
using namespace cv;
using namespace std;
int compX( Point a, Point b)
{
	return a.x - b.x;
}
int compY(Point a, Point b)
{
	return a.y - b.y;
}

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) 
{
	if (name == "median_flow") 
	{
		return std::make_shared<MedianFlowTracker>();
	}
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;
}

bool MedianFlowTracker::Init(const Mat & frame, const Rect & roi)
{
	Mat frame_ = frame;
	Rect position_ = roi;
	return true;
}

Rect MedianFlowTracker::Track(const Mat & frame)
{
	vector<Point> corners;
	vector<Point> corners_out;
	vector<Point> Shift;
	vector<bool> status;
	vector<bool> err;
	goodFeaturesToTrack(frame, corners, 100, 0.3, 7);
	calcOpticalFlowPyrLK(frame_, frame, corners, corners_out, status, err);
	int erased = 0;

	for (int i = 0; i < status.size(); i++)
		if (!status[i])
		{
			corners_out.erase(corners_out.begin() + i - erased);
			corners.erase(corners.begin() + i - erased);
			erased++;
		}

	for(int i = 0; i < corners.size(); i++)
	{
		Shift[i].x = corners[i].x - corners_out[i].x;
		Shift[i].y = corners[i].y - corners_out[i].y;
	}
	sort(Shift.begin(), Shift.end(), compX);
	sort(Shift.begin(), Shift.end(), compY);
	

	position_.x += Shift[Shift.size() / 2].x;
	position_.y += Shift[Shift.size() / 2].y;

	return position_;
}
