#include "detection.hpp"

#include <iostream>
#include "opencv2/objdetect.hpp"


#include <memory>
#include <string>

using namespace std;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
  std::cerr << "Failed to create detector with name '" << name << "'"
            << std::endl;
  return nullptr;
}

bool CascadeDetector::Init(const std::string& model_file_path, cv::CascadeClassifier detector) {
	
	if (detector.load(model_file_path))
		return true;
	else
		return false;
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores) {}