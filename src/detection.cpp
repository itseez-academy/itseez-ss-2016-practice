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

bool CascadeDetector::Init(const std::string& model_file_path)
{
	if (detector.load(model_file_path)==true)
	{
		return true;
	}
	else return false;
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores)
{
	std::vector<int> scores_int;
	detector.detectMultiScale(frame, objects, scores_int);
	scores.resize(scores_int.size());
	for (std::vector<int>::size_type i = 0; i < scores_int.size(); ++i) {
		scores[i] = scores_int[i];
	}
}