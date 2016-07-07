#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace std;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {

	if (name == "median_flow") {
		return std::make_shared<MedianFlowTracker>();
	}
	std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
	return nullptr;
}


bool MedianFlowTracker::Init(const Mat &frame, const Rect &roi)
{
	if (!frame.empty())
	{
		this->frame_ = frame;
	}
	else
	{
		std::cerr << "Error, frame is empty."
			<< std::endl;
		return false;
	}

	if (roi.size() != Size(0,0))
	{
		this->position_ = roi;
	}
	else
	{
		std::cerr << "Error, roi is undefined."
			<< std::endl;
		return false;
	}
	return true;
}


Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	Mat frame_roi_gray;
	vector<Point2f> prev_points_roi;
	vector<Point2f> prev_points;
	vector<Point2f> post_points;
	vector<Point2f> good_new_points;
	vector<Point2f> good_old_points;
	vector<Point2f> differents;
	vector<unsigned char> status;
	vector<int> pointToDelete;
	vector<float> err;
	TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
	int maxPoints = 10;
	cvtColor(this->frame_(position_), frame_roi_gray, CV_BGR2GRAY);
	// ! ! ! Не забыть преобразовать координаты roi в координаты frame
	goodFeaturesToTrack(frame_roi_gray, prev_points_roi, maxPoints,
						0.01, 10, Mat(), 3, false, 0.04);
	//       Не забыть преобразовать координаты roi в координаты frame ! ! !


	// begin roi xy -> frame xy
	for (int i = 0; i < prev_points_roi.size(); i++)
	{
		prev_points.push_back(prev_points_roi[i] + Point2f(position_.x, position_.y));
	}
	// end roi xy -> frame xy


	calcOpticalFlowPyrLK(frame_, frame, prev_points, post_points,
						status, err, Size(31, 31), 3, termcrit, 0, 0.001);
	for (int i = 0; i < status.size(); i++)
	{
		if (status[i]!=0)
		{
			good_new_points.push_back(post_points[i]);
			good_old_points.push_back(prev_points[i]);
			differents.push_back(post_points[i] - prev_points[i]);
		}
	}

	struct sort_class_x
	{
		bool operator() (Point2f i, Point2f j)
		{
			return (i.x<j.x);
		}
	} sort_objectX;

	struct sort_class_y
	{
		bool operator() (Point2f i, Point2f j)
		{
			return (i.y<j.y);
		}
	} sort_objectY;


	//Ниже могут быть ошибки
	//
	//
	Point2f medXY;
	// X median
	sort(differents.begin(), differents.end(), sort_objectX);

	if (differents.size() % 2 != 0)
	{
		medXY.x = (differents[differents.size() / 2].x +
			differents[differents.size() / 2 + 1].x) / 2;

	}
	else
	{
		medXY.x = differents[differents.size() / 2 + 1].x;
	}


	//Y median
	sort(differents.begin(), differents.end(), sort_objectY);

	if (differents.size() % 2 != 0)
	{
		medXY.y = (differents[differents.size() / 2].y +
			differents[differents.size() / 2 + 1].y) / 2;

	}
	else
	{
		medXY.y = differents[differents.size() / 2 + 1].y;
	}




	
	Rect position;
	return position;
}