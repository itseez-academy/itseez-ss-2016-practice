#include "detection.hpp"

#include <iostream>

#include <opencv2/objdetect.hpp>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) 
{
	if (name == "cascade") 
	{
		return std::make_shared<CascadeDetector>();
	}

	std::cerr << "Failed to create detector with name '" << name << "'"
            << std::endl;
	return nullptr;
}

bool CascadeDetector::Init(const std::string& model_file_path)
{
	this->classifier = CascadeClassifier();

	return this->classifier.load(model_file_path);
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<double>& scores)
{
	if (!this->classifier.empty())
	{
		std::vector<int> intScores;

		this->classifier.detectMultiScale(frame, objects, intScores);

		std::copy(intScores.begin(), intScores.end(), scores);
	}
}