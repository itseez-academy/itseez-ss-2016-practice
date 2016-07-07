#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
  std::cerr << "Failed to create detector with name '" << name << "'"
            << std::endl;
  if (name == "cascade") {
	  return std::make_shared<CascadeDetector>();
  }
  return nullptr;
}

bool CascadeDetector::Init(const std::string& model_file_path) {
	if (detector.load(model_file_path) == false) {
		printf("load failed\n");
		return false;
	}
	return true;
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores) {
	if (!detector.CascadeClassifier::empty()) 
		detector.detectMultiScale(frame, objects);
	else
		printf("detector load failed\n");
}