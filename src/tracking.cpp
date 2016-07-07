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

template<class T> bool eraseElements(vector<T> & data, vector<unsigned char> & status) {
	int erase_index = 0;
	for (int i = 0; i < data.size(); i++) {
		if (!status[i]) {
			data.erase(data.begin() + erase_index);
		}
		else {
			erase_index++;
		}
	}
	return data.size() > 0;
};

template<class T> T getMedian(vector<T> & data) {
	std::nth_element(data.begin(), data.begin() + data.size() / 2, data.end());
	return data[data.size() / 2];
}

Rect rescaleRect(Rect& rect, float scale) {
	Rect res = rect;
	int new_width = res.width / scale + 0.5;
	int new_height = res.height / scale + 0.5;
	res.x += (res.width - new_width) / 2;
	res.y += (res.height - new_height) / 2;
	res.width = new_width;
	res.height = new_height;
	return res;
}

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
	const int minCorners = 5;

	Mat mask = Mat::zeros(frame.size(), CV_8UC1);
	rectangle(mask, position_, 255, -1);
	Mat gray_frame_;
	cvtColor(frame_, gray_frame_, CV_BGR2GRAY);
	
	goodFeaturesToTrack(gray_frame_, corners, maxCorners, qualityLevel, minDistance, mask);
	if (corners.size() < minCorners)	return Rect();

	Mat gray_frame;
	cvtColor(frame, gray_frame, CV_BGR2GRAY);

	vector<Point2f> corners_forward, corners_backward;
	vector<unsigned char> status_forward, status_backward; 
	Mat error_forward, error_backward;

	calcOpticalFlowPyrLK(gray_frame_, gray_frame, corners, corners_forward, status_forward, error_forward);	
	eraseElements(corners, status_forward);
	eraseElements(corners_forward, status_forward);
	if (corners.size() < minCorners)	return Rect();

	calcOpticalFlowPyrLK(gray_frame, gray_frame_, corners_forward, corners_backward, status_backward, error_backward);
	eraseElements(corners, status_backward);
	eraseElements(corners_forward, status_backward);
	eraseElements(corners_backward, status_backward);
	if (corners.size() < minCorners)	return Rect();

	vector<float> fb_error(corners.size());
	for (int i = 0; i < fb_error.size(); i++) {
		fb_error[i] = norm(corners[i] - corners_backward[i]);
	}
	float median_error = getMedian(fb_error);

	vector<unsigned char> error_status(fb_error.size());
	for (int i = 0; i < error_status.size(); i++) {
		error_status[i] = fb_error[i] > median_error ? 0 : 1;
	}
	eraseElements(corners, error_status);
	eraseElements(corners_forward, error_status);
	eraseElements(corners_backward, error_status);
	if (corners.size() < minCorners)	return Rect();

	vector<float> shifts_x(corners.size());
	vector<float> shifts_y(corners.size());
	for (int i = 0; i < corners.size(); i++) {
		shifts_x[i] = (corners_forward[i].x - corners[i].x);
		shifts_y[i] = (corners_forward[i].y - corners[i].y);
	}

	position_.x += getMedian(shifts_x) + 0.5;
	position_.y += getMedian(shifts_y) + 0.5;

	frame_ = frame.clone();

	vector<float> scales;
	float old_distance(0), new_distance(0);

	for (int i = 0; i < corners.size(); i++) {
		for (int j = i+1; j < corners.size(); j++) {
			old_distance = norm(corners[i] - corners[j]);
			new_distance = norm(corners_forward[i] - corners_forward[j]);
			scales.push_back(old_distance / new_distance);
		}
	}
	float median_scale = getMedian(scales);

	position_ = rescaleRect(position_, median_scale);

	return position_;
}
