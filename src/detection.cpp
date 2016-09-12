#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {

	if (name == "cascade") {
		return std::make_shared<CascadeDetector>();
	}

  std::cerr << "Failed to create detector with name '" << name << "'"
            << std::endl;
  return nullptr;
}

bool CascadeDetector::Init(const std::string & model_file_path)
{
	if (detector.load(model_file_path))
		return true;
	return false;
}

void CascadeDetector::Detect(const cv::Mat & frame, std::vector<cv::Rect>& objects, std::vector<double>& scores)
{
	detector.detectMultiScale(frame, objects);
}
