#include "tracking.hpp"

#include <iostream>
#include <vector>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
	if (name == "median_flow") {
		return std::make_shared<MedianFlowTracker>();
	}
	std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
	return nullptr;
}

bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi) {
	if (frame.empty())
		return false;
	position_ = roi;
	if (frame_.empty()) {
		xPos = position_.x;
		yPos = position_.y;
	}
	frame.copyTo(frame_);
	cvtColor(frame_, frame_, CV_BGR2GRAY);
	frame_.convertTo(frame_, CV_8UC1);
	
	return true;
}

Rect MedianFlowTracker::Track(const cv::Mat &frame) {
	Mat frame_gray;
	frame.copyTo(frame_gray);
	cvtColor(frame_gray, frame_gray, CV_BGR2GRAY);
	frame_gray.convertTo(frame_gray, CV_8UC1);
	std::vector<Point2f> srcPoint;
	std::vector<Point2f> nextPoint;
	std::vector<uchar> status;
	Mat errors;
	
	Mat frame_roi = frame_(this->position_);
	goodFeaturesToTrack(frame_roi, srcPoint, 50, 0.01, 1.0);
	for (int i = 0; i < srcPoint.size(); i++) {
		srcPoint[i] += Point2f(position_.x, position_.y);
	}
	Mat cir;
	frame.copyTo(cir);
	for (int i = 0; i < srcPoint.size(); i++) {
		circle(cir, srcPoint[i], 1, Scalar(255), 1);
	}
	namedWindow("Point image", WINDOW_NORMAL);
	resizeWindow("Point image", 640, 480);
	imshow("Point image", cir);
	calcOpticalFlowPyrLK(this->frame_, frame_gray, srcPoint, nextPoint, status, errors);

	std::vector<Point2f> offset;
	for (int i = 0; i < nextPoint.size(); i++) {
		if (status[i])
			offset.push_back(nextPoint[i] - srcPoint[i]);
	}
	
	std::vector<float> x, y;
	for (int i = 0; i < offset.size(); i++) {
		x.push_back(offset[i].x);
		y.push_back(offset[i].y);
	}

	sort(x.begin(), x.end());
	sort(y.begin(), y.end());

	float xMedian, yMedian;
	xMedian = x[x.size() / 2];
	yMedian = y[y.size() / 2];

	Rect dstPosition = this->position_;
	//xPos -= xMedian;
	//yPos -= yMedian;
	
	//dstPosition.x = roundf(xPos);
	//dstPosition.y = roundf(yPos);
	dstPosition.x += cvRound(xMedian);
	dstPosition.y += cvRound(yMedian);
	return dstPosition;
}