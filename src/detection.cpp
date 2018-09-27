#include "detection.hpp"

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name) 
{
	if (name == "cascade")
	{
		return std::make_shared<CascadeDetector>();
	}
	else
		return nullptr;

}

bool CascadeDetector::Init(const std::string & model_file_path)
{
	bool flag = false;
	if (ccdetector.load(model_file_path))
		flag = true;
	return flag;
}

void CascadeDetector::Detect(const cv::Mat & frame, std::vector<cv::Rect>& ccdetectorects,
	std::vector<double>& scores)
{
	std::vector<int> numDetections;

	if (ccdetector.empty()) throw "Classifier is empty";
	if (frame.empty()) throw "Matrix is empty";
	if (ccdetector.empty()) throw "Vector of rect is empty";

	ccdetectorects.clear();

	ccdetector.detectMultiScale(frame, ccdetectorects, numDetections);
	
	scores.assign(numDetections.begin(), numDetections.end());

}
