#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;
}


bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi)
{
	if (frame.empty())
		return false;
	if (roi.area() == 0)
		return false;

	position_ = roi;
	frame_ = frame;

	return true;
}

struct sortVecY {
	bool operator() (Vec2f vec1, Vec2f vec2) { return (vec1[1] < vec2[1]); }
} sortY;

struct sortVecX {
	bool operator() (Vec2f vec1, Vec2f vec2) { return (vec1[0] < vec2[0]); }
} sortX;

Rect MedianFlowTracker::Track(const cv::Mat &frame)
{
	std::vector<Point2f> corners;
	std::vector<Point2f> cornersNext;
	std::vector<uchar> status;
	std::vector<float> err;
	int maxCorners = 15;
	double qualityLevel = 0.01;
	double minDistance = 10;

	Mat frame_roi = frame_(position_);

	goodFeaturesToTrack(frame_roi, corners, maxCorners, qualityLevel, minDistance);
	
	for (int i = 0; i < corners.size(); i++)
	{
		corners[i].x += position_.tl().x;
		corners[i].y += position_.tl().y;
	}

	calcOpticalFlowPyrLK(frame_, frame, corners, cornersNext, status, err, Size(10, 10));

	std::vector<Point2f> cornersNew;
	std::vector<Point2f> cornersNextNew;
	int i = 0;

	for_each(status.begin(), status.end(), [&](int stat)
	{
		if (stat)
		{
			cornersNew.push_back(corners[i]);
			cornersNextNew.push_back(cornersNext[i]);
			i++;
		}
	});

	std::vector<Vec2f> vectors;
	std::vector<Vec2f> vectorsSortX;
	std::vector<Vec2f> vectorsSortY;

	for (int i = 0; i < cornersNew.size(); i++)
	{
		vectors.push_back(cornersNextNew[i] - cornersNew[i]);
		vectorsSortX.push_back(vectors[i]);
		vectorsSortY.push_back(vectors[i]);
	}
	
	sort(vectorsSortX.begin(), vectorsSortX.end(), sortX);
	sort(vectorsSortY.begin(), vectorsSortY.end(), sortY);

	int idxMedian = vectors.size() / 2;
	Vec2f medianCenter(vectorsSortX[idxMedian][0], vectorsSortY[idxMedian][1]);

	Point2f pointNew1 = Point2f(position_.x + medianCenter[0], position_.y + medianCenter[1]);
	Point2f pointNew2 = Point2f(position_.x + position_.width + medianCenter[0], position_.y + position_.height + medianCenter[1]);
	Rect positionNext = Rect(pointNew1, pointNew2);

	position_ = positionNext;
	frame_ = frame.clone();

	return position_;
}
