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

template<class T> void erase_elements (vector<T> & data, vector<unsigned char> & status) {
	int erase_index = 0;
	for (int i = 0; i < data.size(); i++) {
		if (!status[i]) {
			data.erase(data.begin() + erase_index);
		}
		else {
			erase_index++;
		}
	}
};

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

	vector<Point2f> corners_forward, corners_backward;
	vector<unsigned char> status_forward, status_backward; 
	Mat error_forward, error_backward;

	calcOpticalFlowPyrLK(gray_frame_, gray_frame, corners, corners_forward, status_forward, error_forward);	
	erase_elements(corners, status_forward);
	erase_elements(corners_forward, status_forward);

	calcOpticalFlowPyrLK(gray_frame, gray_frame_, corners_forward, corners_backward, status_backward, error_backward);
	erase_elements(corners, status_backward);
	erase_elements(corners_forward, status_backward);
	erase_elements(corners_backward, status_backward);

	vector<float> fb_error(corners.size());
	for (int i = 0; i < fb_error.size(); i++) {
		fb_error[i] = (corners[i].x - corners_backward[i].x) * (corners[i].x - corners_backward[i].x) +
			(corners[i].y - corners_backward[i].y) * (corners[i].y - corners_backward[i].y);
	}
	std::nth_element(fb_error.begin(), fb_error.begin() + fb_error.size() / 2, fb_error.end());
	float median_error = fb_error[fb_error.size() / 2];

	vector<unsigned char> error_status(fb_error.size());
	for (int i = 0; i < error_status.size(); i++) {
		error_status[i] = fb_error[i] > median_error ? 0 : 1;
	}
	erase_elements(corners, error_status);
	erase_elements(corners_forward, error_status);
	erase_elements(corners_backward, error_status);

	vector<float> shifts_x(corners.size());
	vector<float> shifts_y(corners.size());
	for (int i = 0; i < corners.size(); i++) {
		shifts_x[i] = (corners_forward[i].x - corners[i].x);
		shifts_y[i] = (corners_forward[i].y - corners[i].y);
	}

	std::nth_element(shifts_x.begin(), shifts_x.begin() + shifts_x.size() / 2, shifts_x.end());
	std::nth_element(shifts_y.begin(), shifts_y.begin() + shifts_y.size() / 2, shifts_y.end());

	position_.x += shifts_x[shifts_x.size() / 2] + 0.5;
	position_.y += shifts_y[shifts_y.size() / 2] + 0.5;

	frame_ = frame.clone();
	return position_;
}
