#include "detection.hpp"

#include <iostream>

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Detector> Detector::CreateDetector(const string& name)
{
	if (name == "cascade") {
		return std::make_shared<CascadeDetector>();
	}
	std::cerr << "Failed to create detector with name '" << name << "'"
            << std::endl;
	return nullptr;
}

bool CascadeDetector::Init(const string& model_file_path)
{
	
	if (!detector.load(model_file_path))
	{ 
	//	printf("--(!)Error loading face cascade\n"); 
		return false; 
	}
	return true;
	//std::cerr << "Failed to create detector with name '" << "'"
	//	<< std::endl;
}

void CascadeDetector::Detect(const cv::Mat& frame, std::vector<cv::Rect>& objects,
	std::vector<int>& scores)
{
//	Mat frame_copy;
//	frame.copyTo(frame_copy);
	if (detector.empty())
	{
		printf("--(!)Error cascade has not loaded \n");
		return;
	}
	//в multiscale надо передать доп. параметры!
	detector.detectMultiScale(frame, objects, scores, 1.1, 3, 0, Size(30,30), Size());
	std::cerr << "Just as planned"
		<< std::endl;
}