#include "detection.hpp"

#include <iostream>
#include "opencv2/objdetect.hpp"


#include <memory>
#include <string>

using namespace std;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
	if (name == "cascade") {
		return std::make_shared<CascadeDetector>();
	}

  std::cerr << "Failed to create detector with name '" << name << "'"
            << std::endl;
  return nullptr;
}

bool CascadeDetector::Init(const std::string& model_file_path) {
	
	if (detector.load(model_file_path))
		return true;
	else
		return false;
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores) {

	if (detector.empty())
		cout << "Model has not been loaded" << endl;
	else {
		std::vector<int> numDetections;
		detector.detectMultiScale(frame, objects, numDetections);
		
		scores.resize(numDetections.size());
		
		std::copy(numDetections.begin(), numDetections.end(), scores.begin());
	}
}