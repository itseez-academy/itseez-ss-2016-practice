#include "tracking.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using std::vector;
using namespace cv;

#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
	if (name == "median_flow") {
		return std::make_shared<MedianFlowTracker>();
	}
	else {
		std::cerr << "Failed to create tracker with name '" << name << "'"
			<< std::endl;
	}
  return nullptr;
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi) {
	frame_ = frame;
	position_ = roi;
	return frame.cols && frame.rows && roi.width && roi.height;
}

Rect MedianFlowTracker::Track(const Mat &frame) {
	vector<Point2f> corners;
	const int maxCorners = 50;
	const double qualityLevel = 0.01;
	const double minDistance = 5.0;
	Mat mask = Mat::zeros(frame.size(), CV_8UC1);
	rectangle(mask, position_, 255, -1);
	Mat gray_frame_;
	cvtColor(frame_, gray_frame_, CV_BGR2GRAY);
	
	goodFeaturesToTrack(gray_frame_, corners, maxCorners, qualityLevel, minDistance, mask);
	if (!corners.size()) return position_;

	Mat gray_frame;
	cvtColor(frame, gray_frame, CV_BGR2GRAY);

	/*Mat markers_frame = frame_.clone();
	for each (auto corner in corners) {
		drawMarker(markers_frame, corner, Scalar(0, 255, 0));
	}
	imshow("markers", markers_frame);
	waitKey(1);*/
	vector<Point2f> new_corners;
	vector<unsigned char> status;
	Mat error;
	calcOpticalFlowPyrLK(gray_frame_, gray_frame, corners, new_corners, status, error);

	vector<float> shifts_x;
	vector<float> shifts_y;
	for (int i = 0; i < corners.size(); i++) {
		if (status[i]) {
			shifts_x.push_back(new_corners[i].x - corners[i].x);
			shifts_y.push_back(new_corners[i].y - corners[i].y);
		}
	}

	std::nth_element(shifts_x.begin(), shifts_x.begin() + shifts_x.size() / 2, shifts_x.end());
	std::nth_element(shifts_y.begin(), shifts_y.begin() + shifts_y.size() / 2, shifts_y.end());

	position_.x += shifts_x[shifts_x.size() / 2] + 0.5;
	position_.y += shifts_y[shifts_y.size() / 2] + 0.5;

	frame_ = frame.clone();
	return position_;
}
