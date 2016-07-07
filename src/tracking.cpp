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
	Mat frame_roi;

	//checking the ranges
	if (position_.x < 0 || position_.y < 0 || 
		position_.x + position_.width >= frame.cols ||
		position_.y + position_.height >= frame.rows)
		return position_;

	//search features
	frame_roi = frame_(this->position_);
	goodFeaturesToTrack(frame_roi, srcPoint, 50, 0.01, 1.0);
	if (srcPoint.size() == 0)
		return position_;
	for (int i = 0; i < srcPoint.size(); i++) {
		srcPoint[i] += Point2f(position_.x, position_.y);
	}
	
	//drawings points
	Mat cir;
	frame.copyTo(cir);
	for (int i = 0; i < srcPoint.size(); i++) {
		circle(cir, srcPoint[i], 1, Scalar(255), 1);
	}
	namedWindow("Point image", WINDOW_NORMAL);
	resizeWindow("Point image", 640, 480);
	imshow("Point image", cir);
	
	//flow
	calcOpticalFlowPyrLK(frame_, frame_gray, srcPoint, nextPoint, status, errors);
	
	//reverse flow
	std::vector<Point2f> nextPointRev;
	std::vector<uchar> statusRev;
	Mat errorsRev;

	calcOpticalFlowPyrLK(frame_gray, frame_, nextPoint, nextPointRev, statusRev, errorsRev);

	//forvard-backward
	std::vector<Point2f> offset;
	std::vector<float> medianErrorX;
	std::vector<float> medianErrorY;
	for (int i = 0; i < nextPointRev.size(); i++) {
		if (statusRev[i] && status[i]) {
			medianErrorX.push_back(nextPointRev[i].x - srcPoint[i].x);
			medianErrorY.push_back(nextPointRev[i].y - srcPoint[i].y);
		}
	}

	//search error median 
	std::vector<float> medianErrorXSort(medianErrorX);
	std::vector<float> medianErrorYSort (medianErrorY);

	sort(medianErrorXSort.begin(), medianErrorXSort.end());
	sort(medianErrorYSort.begin(), medianErrorYSort.end());
	float thresholdX = medianErrorXSort[medianErrorXSort.size() / 2];
	float thresholdY = medianErrorYSort[medianErrorYSort.size() / 2];

	//filter "bad" points
	for (int i = 0; i < medianErrorX.size(); i++) {
		if (statusRev[i] && status[i] && medianErrorX[i] < thresholdX
			&& medianErrorY[i] < thresholdY)
			offset.push_back(nextPoint[i] - srcPoint[i]);
	}
	
	//take median value
	std::vector<float> x, y;
	for (int i = 0; i < offset.size(); i++) {
		x.push_back(offset[i].x);
		y.push_back(offset[i].y);
	}

	sort(x.begin(), x.end());
	sort(y.begin(), y.end());

	float xMedian, yMedian;
	x.size() > 0 ? xMedian = x[x.size() / 2] : xMedian = 0;
	y.size() > 0 ? yMedian = y[y.size() / 2] : yMedian = 0;

	//new rectangle for next frame
	Rect dstPosition = this->position_;

	dstPosition.x += cvRound(xMedian);
	dstPosition.y += cvRound(yMedian);
	return dstPosition;
}