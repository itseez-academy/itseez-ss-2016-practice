#include "tracking.hpp"

#include <iostream>

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

double MedianFlowTracker::medianFunc(std::vector<double> vec) {
	if (vec.empty()) throw - 1;
	if (vec.size() % 2 == 0) {
		auto median_it1 = vec.begin() + vec.size() / 2 - 1;
		auto median_it2 = vec.begin() + vec.size() / 2;

		std::nth_element(vec.begin(), median_it1, vec.end());
		auto p1 = *median_it1;

		std::nth_element(vec.begin(), median_it2, vec.end());
		auto p2 = *median_it2;

		return (p1 + p2) / 2;

	}
	else {
		const auto median_it = vec.begin() + vec.size() / 2;
		std::nth_element(vec.begin(), median_it, vec.end());
		return *median_it;
	}
}
void MedianFlowTracker::delElements(std::initializer_list < std::vector<Point2f>*> list, std::vector<uchar>* status,
	std::vector<double>* revErr, double value) {
	
	int size = status ? status->size() : revErr->size();
	int i = 0;
	for (int j = 0; j < size; j++)
		if ((status && *(status->begin() + j)) || (revErr && *(revErr->begin() + j) > value)) {
			for (auto k = list.begin(); k != list.end(); k++)
				((*k)->begin() + i) = ((*k)->begin() + j);
			i++;
		}
	for (auto k = list.begin(); k != list.end(); k++)
		(*k)->resize(i);	
}


bool MedianFlowTracker::Init(const cv::Mat &frame, const cv::Rect &roi) {
	if (frame.empty()) return false;
	frame_ = frame;
	position_ = roi;
	return true;
}
cv::Rect MedianFlowTracker::Track(const cv::Mat &frame) {
	Mat obj;
	std::vector<Point2f> corners;
	(frame(position_)).copyTo(obj);
	cvtColor(frame(position_), obj, COLOR_BGR2GRAY);
	goodFeaturesToTrack(obj, corners, 100, 0.03, 5);
	if (corners.empty()) throw "corners not found";	
	std::vector<Point2f> nextPts;
	std::vector<uchar> status;
	std::vector<float> err;
	Mat gray_frame;
	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);
	Mat cir = obj.clone();
	for (int i = 0; i < corners.size(); i++) 
		corners[i] += Point2f(position_.x, position_.y);
	calcOpticalFlowPyrLK(frame_, gray_frame, corners, nextPts, status, err);
	std::initializer_list < std::vector<Point2f>*> list =  { &nextPts, &corners};
	delElements(list, &status);
	std::vector<Point2f> revPts;
	calcOpticalFlowPyrLK(gray_frame, frame_, nextPts, revPts, status, err);
	list = { &nextPts, &revPts, &corners };
	delElements(list, &status);
	if (corners.size() < 2)
		throw "no corners";

	std::vector<double> revErr;
	for (int j = 0; j < corners.size(); j++)
		revErr.emplace_back(norm(corners[j] - revPts[j]));
	list = { &nextPts, &revPts, &corners };
	delElements(list, 0, &revErr, medianFunc(revErr));

	std::vector<double> diffX, diffY;
	for (int j = 0; j < corners.size(); j++) {
		diffX.emplace_back(nextPts[j].x - revPts[j].x);
		diffY.emplace_back(nextPts[j].y - revPts[j].y);
	}

	Point2f newPoint(medianFunc(diffX),medianFunc(diffY));
	std::vector<double> coeffs;

	for (int i = 0; i < corners.size(); i++) {
		for (int j = i + 1; j < corners.size(); j++) {
			double nextNorm = norm(nextPts[i] - nextPts[j]);
			double prevNorm = norm(corners[i] - corners[j]);
			coeffs.emplace_back(nextNorm / prevNorm);
		}
	}
	double coeff;
	try {
		coeff = medianFunc(coeffs);
	}
	catch (int a) {
		coeff = 1;
	}

	Rect next_position = position_ + Point(newPoint);
	next_position.width *= coeff;
	next_position.height *= coeff;
	position_ = next_position;
	frame_ = gray_frame;
	return next_position;
}