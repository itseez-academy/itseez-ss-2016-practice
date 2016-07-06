#include "detection.hpp"

#include <iostream>
#include <string>

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

	detector.CascadeClassifier::load(model_file_path);
	if ( !detector.CascadeClassifier::load(model_file_path) ) {
		printf("--(!)Error loading file cascade\n");
		return false; 
	}
	else
	{
		return true;
	}
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores) {

	if (!frame.empty()) {
		std::vector<int> numDetected;
		detector.CascadeClassifier::detectMultiScale(frame, objects, numDetected);
		scores.resize(numDetected.size());
		std::copy(numDetected.begin(), numDetected.end(), scores.begin());
		std::cout << scores.size() << std::endl;
	}
}