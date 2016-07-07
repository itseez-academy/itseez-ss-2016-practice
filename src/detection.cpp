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

bool CascadeDetector::Init(const std::string& model_file_path)
{
	detector = cv::CascadeClassifier(model_file_path);
	return true;
}

void CascadeDetector::Detect(const cv::Mat & frame, std::vector<cv::Rect>& objects, std::vector<double>& scores)
{
	std::vector<int> scoresi;
	if (detector.empty())
	{
		return;
	}
	detector.detectMultiScale(frame, objects,scoresi);
	scores.resize(scoresi.size());
	copy(scoresi.begin(), scoresi.end(), scores.begin());
}

