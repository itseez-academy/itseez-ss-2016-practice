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
	detector.load(model_file_path);
	if (detector.empty())
	{
		std::cerr << "model not loaded" << std::endl;
		return false;
	}
	std::cerr << "model loaded" << std::endl;
	return true;
};

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores) {
	if (frame.empty()) {
		std::cerr << "frame empty" << std::endl;
	}
	if (detector.empty())
		return;
	
	std::vector<int> scor;
	detector.detectMultiScale(frame, objects, scor, 1.05, 1);
	std::cerr << "object size = " << objects.size() << std::endl;
	scores.resize(scor.size());
	std::copy(scor.begin(), scor.end(), scores.begin());
};