#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) {
  std::cerr << "Failed to create detector with name '" << name << "'"
            << std::endl;
  return nullptr;
}

bool CascadeDetector::Init(const std::string& model_file_path) {

	try
	{
		detector = CascadeClassifier(model_file_path);
	}
	catch (const std::exception& ex) 
	{
		return false;
	}

	return true;
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores)
{
	if (detector.empty())
		return;

	std::vector<int> scoresInt;
	detector.detectMultiScale(frame, objects, scoresInt);

	for_each(scoresInt.begin(), scoresInt.end(), [&](int _n)
	{
		scores.push_back(_n);
	});
}
