#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using std::vector;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
	if (name == "cascade") {
		return std::make_shared<CascadeDetector>();
	}
	else {
		std::cerr << "Failed to create detector with name '" << name << "'"
			<< std::endl;
		return nullptr;
	}
}

bool CascadeDetector::Init(const std::string& model_file_path) {
	return detector.load(model_file_path);
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores) {
	if (!detector.empty()) {
		vector<int> dscores;
		detector.detectMultiScale(frame, objects, dscores);
		scores.resize(dscores.size());
		for (int i = 0; i < dscores.size(); i++) {
			scores[i] = (double)dscores[i];
		}
	}
}